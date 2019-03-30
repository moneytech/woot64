#pragma once

#include <device.hpp>
#include <list.hpp>
#include <mutex.hpp>
#include <sequencer.hpp>
#include <types.h>

class Drive : public Device
{
    static Sequencer<int> id;
    static List<Drive *> drives;
    static Mutex listLock;

    static bool lockList();
    static void unLockList();
    static bool add(Drive *drive);
    static bool remove(Drive *drive);
protected:
    Drive(Device *parent, size_t sectorSize, uint64_t sectorCount, const char *model, const char *serial);
public:
    int Id;
    size_t SectorSize;
    uint64_t SectorCount;
    uint64_t Size;
    char *Model;
    char *Serial;

    static Drive *GetById(int id);

    virtual int64_t ReadSectors(void *buffer, uint64_t start, int64_t count);
    virtual int64_t WriteSectors(const void *buffer, uint64_t start, int64_t count);
    virtual bool HasMedia();
    virtual ~Drive();

    virtual const char *GetDisplayName() override;
};
