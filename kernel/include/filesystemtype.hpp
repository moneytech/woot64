#pragma once

#include <list.hpp>
#include <mutex.hpp>
#include <types.h>

class Volume;

class FileSystemType
{
    static Mutex listLock;
    static List<FileSystemType *> fsTypes;

    static bool lockList();
    static void unLockList();
    static bool append(FileSystemType *type);
    static bool remove(FileSystemType *type);
protected:
    FileSystemType(const char *name);
public:
    char *Name;

    static bool ForEach(bool (*callback)(FileSystemType *type, void *arg), void *arg);

    virtual int Detect(Volume *volume);
    virtual ~FileSystemType();
};
