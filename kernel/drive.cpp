#include <cpu.hpp>
#include <drive.hpp>
#include <errno.h>
#include <string.hpp>
#include <stringbuilder.hpp>

Sequencer<int> Drive::id(0);
List<Drive *> Drive::drives;
Mutex Drive::listLock(false, "Drive::listLock");

bool Drive::lockList()
{
    return listLock.Acquire(1000, false);
}

void Drive::unLockList()
{
    return listLock.Release();
}

bool Drive::append(Drive *drive)
{
    if(!lockList()) return false;
    drives.Append(drive);
    unLockList();
    return true;
}

bool Drive::remove(Drive *drive)
{
    if(!lockList()) return false;
    uint res = drives.Remove(drive, nullptr, false);
    unLockList();
    return res != 0;
}

Drive::Drive(Device *parent, size_t sectorSize, uint64_t sectorCount, const char *model, const char *serial) :
    Device(parent),
    Id(id.GetNext()),
    SectorSize(sectorSize),
    SectorCount(sectorCount),
    Size(SectorSize * SectorCount),
    Model(model ? String::Duplicate(model) : nullptr),
    Serial(serial ? String::Duplicate(serial) : nullptr)
{
    append(this);
}

Drive *Drive::GetById(int id)
{
    if(!lockList()) return nullptr;
    Drive *res = nullptr;
    for(Drive *drive : drives)
    {
        if(drive->Id == id)
        {
            res = drive;
            break;
        }
    }
    unLockList();
    return res;
}

bool Drive::ForEach(bool (*callback)(Drive *, void *), void *arg)
{
    if(!lockList()) return false;
    for(Drive *drive : drives)
    {
        if(callback(drive, arg))
            break;
    }
    unLockList();
    return true;
}

int64_t Drive::ReadSectors(void *buffer, uint64_t start, uint64_t count)
{
    return -ENOSYS;
}

int64_t Drive::WriteSectors(const void *buffer, uint64_t start, uint64_t count)
{
    return -ENOSYS;
}

bool Drive::HasMedia()
{
    return true;
}

Drive::~Drive()
{
    remove(this);
    if(Model) delete[] Model;
    if(Serial) delete[] Serial;
}

const char *Drive::GetDisplayName()
{
    return "Generic drive";
}
