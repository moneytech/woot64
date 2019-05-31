#include <drive.hpp>
#include <errno.h>
#include <string.hpp>
#include <stringbuilder.hpp>
#include <volume.hpp>
#include <volumetype.hpp>

Sequencer<int> Volume::ids(0);
Mutex Volume::listLock(false, "Volume::listLock");
List<Volume *> Volume::volumes;

bool Volume::lockList()
{
    return listLock.Acquire(1000, false);
}

void Volume::unLockList()
{
    listLock.Release();
}

bool Volume::append(Volume *volume)
{
    if(!lockList()) return false;
    volumes.Append(volume);
    unLockList();
    return true;
}

bool Volume::remove(Volume *volume)
{
    if(!lockList()) return false;
    uint res = volumes.Remove(volume, nullptr, false);
    unLockList();
    return res != 0;
}

Volume::Volume(Drive *drive, VolumeType *type) :
    Id(ids.GetNext()), drive(drive), type(type), FS(nullptr)
{
    append(this);
}

int Volume::DetectAll()
{
    int found = 0;
    Drive::ForEach([](Drive *drive, void *arg) -> bool
    {
        int *found = (int *)arg;
        struct VTArg
        {
            int *found;
            Drive *drive;
        } vtArg = { found, drive };
        VolumeType::ForEach([](VolumeType *type, void *arg) -> bool
        {
            VTArg *vtArg = (VTArg *)arg;
            int res = type->Detect(vtArg->drive);
            if(res <= 0) return false;
            *vtArg->found += res;
            return false;
        }, &vtArg);
        return false;
    }, &found);
    return found;
}

bool Volume::ForEach(bool (*callback)(Volume *, void *), void *arg)
{
    if(!lockList()) return false;
    for(Volume *volume : volumes)
    {
        if(callback(volume, arg))
            break;
    }
    unLockList();
    return true;
}

#include <debug.hpp>

void Volume::SynchronizeAll()
{
    lockList();
    for(Volume *volume : volumes)
        volume->Synchronize();
    unLockList();
}

size_t Volume::GetSectorSize()
{
    return drive->SectorSize;
}

uint64_t Volume::GetSectorCount()
{
    return drive->SectorCount;
}

size_t Volume::ReadSectors(void *buffer, uint64_t firstSector, size_t n)
{
    return -ENOSYS;
}

size_t Volume::WriteSectors(const void *buffer, uint64_t firstSector, size_t n)
{
    return -ENOSYS;
}

size_t Volume::Read(void *buffer, uint64_t position, size_t n)
{
    return -ENOSYS;
}

size_t Volume::Write(const void *buffer, uint64_t position, size_t n)
{
    return -ENOSYS;
}

int Volume::Synchronize()
{
    return 0;
}

Volume::~Volume()
{
    remove(this);
}
