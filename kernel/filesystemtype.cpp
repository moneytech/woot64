#include <debug.hpp>
#include <filesystemtype.hpp>
#include <string.hpp>

Mutex FileSystemType::listLock(false, "FileSystemType::listLock");
List<FileSystemType *> FileSystemType::fsTypes;

bool FileSystemType::lockList()
{
    return listLock.Acquire(1000, false);
}

void FileSystemType::unLockList()
{
    listLock.Release();
}

bool FileSystemType::append(FileSystemType *type)
{
    if(!lockList()) return false;
    fsTypes.Append(type);
    unLockList();
    return true;
}

bool FileSystemType::remove(FileSystemType *type)
{
    if(!lockList()) return false;
    uint res = fsTypes.Remove(type, nullptr, false);
    unLockList();
    return res != 0;
}

FileSystemType::FileSystemType(const char *name) :
    Name(String::Duplicate(name))
{
    append(this);
}

bool FileSystemType::ForEach(bool (*callback)(FileSystemType *, void *), void *arg)
{
    if(!lockList()) return false;
    for(FileSystemType *type : fsTypes)
    {
        if(callback(type, arg))
            break;
    }
    unLockList();
    return true;
}

int FileSystemType::Detect(Volume *volume)
{
    return 0;
}

FileSystemType::~FileSystemType()
{
    remove(this);
    if(Name) delete[] Name;
}
