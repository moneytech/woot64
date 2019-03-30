#pragma once

#include <sequencer.hpp>
#include <types.h>

class Drive;
class FileSystem;
class VolumeType;

class Volume
{
    static Sequencer<int> ids;
protected:
    int id;
    Drive *drive;
    VolumeType *type;

    Volume(Drive *drive, VolumeType *type);
public:
    FileSystem *FS;

    static int DetectAll();

    virtual size_t GetSectorSize();
    virtual uint64_t GetSectorCount();
    virtual size_t ReadSectors(void *buffer, uint64_t firstSector, size_t n);
    virtual size_t WriteSectors(const void *buffer, uint64_t firstSector, size_t n);
    virtual size_t Read(void *buffer, uint64_t position, size_t n);
    virtual size_t Write(const void *buffer, uint64_t position, size_t n);
    virtual int Synchronize();
};
