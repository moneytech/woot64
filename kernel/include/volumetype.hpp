#pragma once

#include <list.hpp>
#include <mutex.hpp>
#include <types.h>

class Drive;

class VolumeType
{
    static Mutex listLock;
    static List<VolumeType *> volTypes;

    static bool lockList();
    static void unLockList();
    static bool append(VolumeType *type);
    static bool remove(VolumeType *type);
protected:
    VolumeType(const char *name);
public:
    char *Name;

    static bool ForEach(bool (*callback)(VolumeType *type, void *arg), void *arg);

    virtual int Detect(Drive *drive);
    virtual ~VolumeType();
};
