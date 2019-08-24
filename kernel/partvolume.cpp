#include <debug.hpp>
#include <drive.hpp>
#include <errno.h>
#include <misc.hpp>
#include <paging.hpp>
#include <partvolume.hpp>
#include <uuid.hpp>

#define GPT_SIGNATURE_VALUE 0x5452415020494645ULL

#pragma pack(push, 1)
struct MBRTableEntry
{
    uint8_t Flags;
    uint8_t StartCHS[3];
    uint8_t ID;
    uint8_t EndCHS[3];
    uint32_t StartLBA;
    uint32_t LBACount;
};

struct GPTHeader
{
    union
    {
        char Signature[8];
        uint64_t SignatureValue;
    };
    uint16_t VersionMinor;
    uint16_t VersionMajor;
    uint32_t HeaderSize;
    uint32_t HeaderCRC;
    uint32_t Reserved;
    uint64_t CurrentLBA;
    uint64_t BackupLBA;
    uint64_t FirstUsableLBA;
    uint64_t LastUsableLBA;
    UUID DiskUUID;
    uint64_t PartTableLBA;
    uint32_t PartCount;
    uint32_t PartEntrySize;
    uint32_t PartTableCRC;
};

struct GPTEntry
{
    UUID TypeUUID;
    UUID PartitionUUID;
    uint64_t FirstLBA;
    uint64_t LastLBA;
    uint64_t Flags;
    uint16_t Name[36];
};
#pragma pack(pop)

PartVolumeType::PartVolumeType() :
    VolumeType("partvolume")
{
}

int PartVolumeType::Detect(Drive *drive)
{
    if(drive->SectorSize < 512)
        return 0;

    uint8_t *firstSector = reinterpret_cast<uint8_t *>(Paging::AllocDMA(drive->SectorSize));
    if(drive->ReadSectors(firstSector, 0, 1) != 1)
    {
        DEBUG("[partvolume] Couldn't read first sector on drive '%d'\n", drive->Id);
        Paging::FreeDMA(firstSector);
        return -EIO;
    }

    // not sure if this is correct (if there can be mbrs without 55aa)
    if(firstSector[510] != 0x55 || firstSector[511] != 0xAA)
    {
        Paging::FreeDMA(firstSector);
        return 0;
    }

    MBRTableEntry *mbrt = reinterpret_cast<MBRTableEntry *>(firstSector + 0x1BE);

    bool mbrOk = true;
    int activeCount = 0;
    int entryCount = 0;
    for(int i = 0; i < 4; ++i)
    {
        if(!mbrt[i].StartLBA)
            continue;
        if(mbrt[i].Flags & 0x80)
            ++activeCount;
        if((mbrt[i].StartLBA + mbrt[i].LBACount) > drive->SectorCount)
        {
            mbrOk = false;
            break;
        }
        // TODO: Add CHS and LBA equality check
        ++entryCount;
    }

    if(drive->SectorSize != 512 && mbrt[0].ID == 0xCD)
        mbrOk = false; // we have hybrid cd image

    int found = 0;

    bool gptOk = false;
    if(mbrt[0].ID == 0xEE)
    {   // we have GPT partitioning
        mbrOk = false;
        void *headerData = Paging::AllocDMA(drive->SectorSize);
        if(drive->ReadSectors(headerData, 1, 1) != 1)
        {
            DEBUG("[partvolume] Couldn't read GPT header on drive '%d'\n", drive->Id);
            Paging::FreeDMA(headerData);
            Paging::FreeDMA(firstSector);
            return -EIO;
        }
        GPTHeader *header = reinterpret_cast<GPTHeader *>(headerData);
        if(header->SignatureValue == GPT_SIGNATURE_VALUE)
        {
            gptOk = true;
            //DEBUG("[partvolume] GPT version %u.%u\n", header->VersionMajor, header->VersionMinor);
            size_t tableSize = align(header->PartCount * header->PartEntrySize, drive->SectorSize);
            uint8_t *tableData = reinterpret_cast<uint8_t *>(Paging::AllocDMA(tableSize));
            uint64_t tableLBAs = tableSize / drive->SectorSize;
            if(drive->ReadSectors(tableData, header->PartTableLBA, tableLBAs) != tableLBAs)
            {
                DEBUG("[partvolume] Couldn't read GPT partition table on drive '%d'\n", drive->Id);
                Paging::FreeDMA(tableData);
                Paging::FreeDMA(headerData);
                Paging::FreeDMA(firstSector);
                return -EIO;
            }

            for(uint i = 0; i < header->PartCount; ++i)
            {
                GPTEntry *entry = reinterpret_cast<GPTEntry *>(tableData + i * header->PartEntrySize);
                if(entry->TypeUUID == UUID::nil)
                    continue;

                /*PartVolume *vol =*/ new PartVolume(drive, this, entry->FirstLBA, entry->LastLBA - entry->FirstLBA + 1);
                DEBUG("[partvolume] Found GPT parition on drive '%d'\n", drive->Id);
                ++found;
            }
            Paging::FreeDMA(tableData);
        }
        Paging::FreeDMA(headerData);
    }

    if(!entryCount || activeCount > 1)
        mbrOk = false; // no partitions or more than one active partition

    if(!mbrOk && !gptOk)
    {   // unknown partition scheme or not partitioned drive
        Paging::FreeDMA(firstSector);
        /*PartVolume *vol =*/ new PartVolume(drive, this, 0, drive->SectorCount);
        DEBUG("[partvolume] Drive '%d' seems to be not partitioned\n", drive->Id);
        return 1;
    }

    if(mbrOk)
    {
        for(int i = 0; i < 4; ++i)
        {
            if(!mbrt[i].StartLBA)
                continue;

            /*PartVolume *vol =*/ new PartVolume(drive, this, mbrt[i].StartLBA, mbrt[i].LBACount);
            DEBUG("[partvolume] Found MBR parition on drive '%d'\n", drive->Id);
            ++found;
        }
    }

    Paging::FreeDMA(firstSector);
    return found;
}

PartVolumeType *PartVolume::volType = nullptr;

void PartVolume::Initialize()
{
    volType = new PartVolumeType();
}

void PartVolume::Cleanup()
{
    if(volType) delete volType;
}

PartVolume::PartVolume(Drive *parentDrive, VolumeType *vt, uint64_t firstSector, uint64_t sectorCount) :
    BufferedVolume(parentDrive, vt, 32, 128),
    firstSector(firstSector),
    sectorCount(sectorCount)
{
}

uint64_t PartVolume::GetSectorCount()
{
    return sectorCount;
}

size_t PartVolume::ReadSectors(void *buffer, uint64_t firstSector, size_t n)
{
    size_t ss = drive->SectorSize;
    return Read(buffer, (this->firstSector + firstSector) * ss, n * ss);
}

size_t PartVolume::WriteSectors(const void *buffer, uint64_t firstSector, size_t n)
{
    size_t ss = drive->SectorSize;
    return Write(buffer, (this->firstSector + firstSector) * ss, n * ss);
}

size_t PartVolume::Read(void *buffer, uint64_t position, size_t n)
{
    size_t ss = drive->SectorSize;
    return BufferedVolume::Read(buffer, position + firstSector * ss, n);
}

size_t PartVolume::Write(const void *buffer, uint64_t position, size_t n)
{
    size_t ss = drive->SectorSize;
    return BufferedVolume::Write(buffer, position + firstSector * ss, n);
}
