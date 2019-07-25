#pragma once

#include <bufferedvolume.hpp>
#include <partvolume.hpp>
#include <types.h>
#include <volumetype.hpp>

class PartVolumeType : public VolumeType
{
public:
    PartVolumeType();
    virtual int Detect(Drive *drive);
};

class PartVolume : public BufferedVolume
{
    static PartVolumeType *volType;

    uint64_t firstSector;
    uint64_t sectorCount;
public:
    static void Initialize();
    static void Cleanup();

    PartVolume(Drive *parentDrive, VolumeType *vt, uint64_t firstSector, uint64_t sectorCount);

    virtual uint64_t GetSectorCount() override;
    virtual size_t ReadSectors(void *buffer, uint64_t firstSector, size_t n) override;
    virtual size_t WriteSectors(const void *buffer, uint64_t firstSector, size_t n) override;
    virtual size_t Read(void *buffer, uint64_t position, size_t n) override;
    virtual size_t Write(const void *buffer, uint64_t position, size_t n) override;
};
