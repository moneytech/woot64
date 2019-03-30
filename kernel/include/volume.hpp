#pragma once

#include <list.hpp>
#include <mutex.hpp>
#include <sequencer.hpp>
#include <types.h>

class Drive;
class FileSystem;
class VolumeType;

class Volume
{
    static Sequencer<int> ids;
    static Mutex listLock;
    static List<Volume *> volumes;

    static bool lockList();
    static void unLockList();
    static bool append(Volume *volume);
    static bool remove(Volume *volume);
protected:
    Drive *drive;
    VolumeType *type;

    Volume(Drive *drive, VolumeType *type);
public:
    int Id;
    FileSystem *FS;

    static int DetectAll();
    static bool ForEach(bool (*callback)(Volume *volume, void *arg), void *arg);

    virtual size_t GetSectorSize();
    virtual uint64_t GetSectorCount();
    virtual size_t ReadSectors(void *buffer, uint64_t firstSector, size_t n);
    virtual size_t WriteSectors(const void *buffer, uint64_t firstSector, size_t n);
    virtual size_t Read(void *buffer, uint64_t position, size_t n);
    virtual size_t Write(const void *buffer, uint64_t position, size_t n);
    virtual int Synchronize();
    virtual ~Volume();
};
