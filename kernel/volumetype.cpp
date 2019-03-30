#include <string.hpp>
#include <volumetype.hpp>

Mutex VolumeType::listLock(false, "VolumeType::listLock");
List<VolumeType *> VolumeType::volTypes;

bool VolumeType::lockList()
{
    return listLock.Acquire(1000, false);
}

void VolumeType::unLockList()
{
    listLock.Release();
}

bool VolumeType::append(VolumeType *type)
{
    if(!lockList()) return false;
    volTypes.Append(type);
    unLockList();
    return true;
}

bool VolumeType::remove(VolumeType *type)
{
    if(!lockList()) return false;
    uint res = volTypes.Remove(type, nullptr, false);
    unLockList();
    return res != 0;
}

VolumeType::VolumeType(const char *name) :
    Name(String::Duplicate(name))
{
    append(this);
}

bool VolumeType::ForEach(bool (*callback)(VolumeType *, void *), void *arg)
{
    if(!lockList()) return false;
    for(VolumeType *type : volTypes)
    {
        if(callback(type, arg))
            break;
    }
    unLockList();
    return true;
}

int VolumeType::Detect(Drive *drive)
{
    return 0;
}

VolumeType::~VolumeType()
{
    remove(this);
    if(Name) delete[] Name;
}
