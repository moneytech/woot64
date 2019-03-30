#include <debug.hpp>
#include <drive.hpp>
#include <errno.h>
#include <paging.hpp>
#include <partvolume.hpp>

#pragma pack(push, 1)
typedef struct MBRTableEntry
{
    uint8_t Flags;
    uint8_t StartCHS[3];
    uint8_t ID;
    uint8_t EndCHS[3];
    uint32_t StartLBA;
    uint32_t LBACount;
} MBRTableEntry;
#pragma pack(pop)

PartVolumeType::PartVolumeType() :
    VolumeType("partvolume")
{
}

int PartVolumeType::Detect(Drive *drive)
{
    if(drive->SectorSize < 512)
        return 0;

    uint8_t *firstSector = (uint8_t *)Paging::AllocDMA(drive->SectorSize);
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

    MBRTableEntry *mbrt = (MBRTableEntry *)(firstSector + 0x1BE);

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

    if(mbrt[0].ID == 0xEE)
        mbrOk = false; // we have GPT partitioning

    if(!entryCount || activeCount > 1)
        mbrOk = false; // no partitions or more than one active partition

    if(!mbrOk)
    {   // unkonwn partition scheme or not partitioned drive
        Paging::FreeDMA(firstSector);
        PartVolume *vol = new PartVolume(drive, this, 0, drive->SectorCount);
        DEBUG("[partvolume] Drive '%d' seems to be not partitioned\n", drive->Id);
        return 1;
    }

    int found = 0;
    for(int i = 0; i < 4; ++i)
    {
        if(!mbrt[i].StartLBA)
            continue;

        PartVolume *vol = new PartVolume(drive, this, mbrt[i].StartLBA, mbrt[i].LBACount);
        DEBUG("[partvolume] Found MBR parition on drive '%d'\n", drive->Id);
        ++found;
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

PartVolume::PartVolume(Drive *drive, VolumeType *vt, uint64_t firstSector, uint64_t sectorCount) :
    BufferedVolume(drive, vt, 16, 16),
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
