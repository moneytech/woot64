#include <debug.hpp>
#include <dentry.hpp>
#include <errno.h>
#include <filesystem.hpp>
#include <filesystemtype.hpp>
#include <inode.hpp>
#include <string.hpp>
#include <stringbuilder.hpp>
#include <volume.hpp>

Sequencer<int> FileSystem::ids(0);
Mutex FileSystem::mutex(true, "FileSystem::mutex");
List<DEntry *> FileSystem::dentryCache;
List<INode *> FileSystem::inodeCache;
List<FileSystem *> FileSystem::fileSystems;
Mutex FileSystem::listLock(false, "FileSystem::listLock");

bool FileSystem::lockList()
{
    return listLock.Acquire(1000, false);
}

void FileSystem::unLockList()
{
    listLock.Release();
}

bool FileSystem::append(FileSystem *fs)
{
    if(!lockList()) return false;
    fileSystems.Append(fs);
    unLockList();
    return true;
}

bool FileSystem::remove(FileSystem *fs)
{
    if(!lockList()) return false;
    uint res = fileSystems.Remove(fs, nullptr, false);
    unLockList();
    return res != 0;
}

FileSystem::FileSystem(Volume *volume, FileSystemType *type) :
    id(ids.GetNext()), volume(volume), type(type)
{
    append(this);
}

void FileSystem::Initialize()
{
    // do nothing
}

void FileSystem::Cleanup()
{
    Lock();
    for(FileSystem *fs : fileSystems)
    {
        if(fs->volume)
            fs->volume->FS = nullptr;
        delete fs;
    }
    for(DEntry *dentry : dentryCache)
        DEBUG("[filesystem] WARNING: DEntry '%s' still in cache! (ref count: %d)\n", dentry->Name, dentry->ReferenceCount);
    for(INode *inode : inodeCache)
        DEBUG("[filesystem] WARNING: INode still in cache! (ref count: %d)\n", inode->ReferenceCount);
    UnLock();
}

bool FileSystem::Lock()
{
    return mutex.Acquire(10000);
}

void FileSystem::UnLock()
{
    mutex.Release();
}

int FileSystem::DetectAll()
{
    int found = 0;
    Volume::ForEach([](Volume *volume, void *arg) -> bool
    {
        if(volume->FS)
            return false; // this volume already has FS set; skip
        int *found = (int *)arg;
        struct FSTArg
        {
            int *found;
            Volume *volume;
        } fstArg = { found, volume };
        FileSystemType::ForEach([](FileSystemType *type, void *arg) -> bool
        {
            FSTArg *vtArg = (FSTArg *)arg;
            int res = type->Detect(vtArg->volume);
            if(res <= 0) return false;
            *vtArg->found += res;
            return false;
        }, &fstArg);
        return false;
    }, &found);
    return found;
}

void FileSystem::AddDEntry(DEntry *dentry)
{
    Lock();
    dentryCache.Prepend(dentry);
    UnLock();
}

void FileSystem::RemoveDEntry(DEntry *dentry)
{
    Lock();
    dentryCache.Remove(dentry, nullptr, false);
    UnLock();
}

FileSystem *FileSystem::GetByName(const char *name)
{
    if(!lockList()) return nullptr;
    FileSystem *res = nullptr;
    for(FileSystem *fs : fileSystems)
    {
        const char *label = fs->GetLabel();
        bool match = label && !String::Compare(name, label);
        if(!match)
        {
            StringBuilder sb(31);
            sb.WriteFmt("%d", fs->GetId());
            match = !String::Compare(name, sb.String());
        }
        if(match)
        {
            res = fs;
            break;
        }
    }
    unLockList();
    return res;
}

void FileSystem::SynchronizeAll()
{
    lockList();
    for(FileSystem *fs : fileSystems)
        fs->Synchronize();
    unLockList();
}

void FileSystem::PutINode(INode *inode)
{
    if(!inode || !Lock()) return;
    if(inode->ReferenceCount > 0)
        --inode->ReferenceCount;
    if(inode->ReferenceCount <= 0)
    {
        inodeCache.Remove(inode, nullptr, false);
        if(inode->Dirty)
            inode->FS->WriteINode(inode);
        inode->Release();
        delete inode;
    }
    UnLock();
}

DEntry *FileSystem::LookupDEntry(DEntry *parent, const char *name)
{
    if(!Lock() || !parent || !parent->INode || !parent->INode->FS || !name)
        return nullptr;
    for(DEntry *dentry : dentryCache)
    {
        if(parent == dentry->Parent && !String::Compare(name, dentry->Name))
        {   // if dentry already in cache
            ++dentry->ReferenceCount;
            UnLock();
            return dentry;
        }
    }

    ino_t ino = parent->INode->Lookup(name);
    if(ino <= 0)
    {
        UnLock();
        return nullptr;
    }
    DEntry *dentry = new DEntry(name, parent, parent->INode->FS->GetINode(ino));
    UnLock();
    return dentry;
}

DEntry *FileSystem::GetDEntry(DEntry *dentry)
{
    if(!Lock()) return nullptr;
    DEntry *res = dentryCache.Find(dentry, nullptr);
    if(!res)
    {
        UnLock();
        return nullptr;
    }
    ++res->ReferenceCount;
    UnLock();
    return res;
}

void FileSystem::PutDEntry(DEntry *dentry)
{
    if(!dentry || !Lock()) return;
    //DEBUG("putdentry: %s\n", dentry->Name);
    if(!(dentry->ReferenceCount--)) delete dentry;
    UnLock();
}

INode *FileSystem::GetINode(ino_t number)
{
    if(!Lock()) return nullptr;
    for(INode *inode : inodeCache)
    {
        if(inode->FS == this && inode->Number == number)
        {
            ++inode->ReferenceCount;
            UnLock();
            return inode;
        }
    }
    INode *inode = ReadINode(number);
    if(!inode)
    {
        UnLock();
        return nullptr;
    }
    inodeCache.Prepend(inode);
    ++inode->ReferenceCount;
    UnLock();
    return inode;
}

int FileSystem::GetId()
{
    return id;
}

void FileSystem::SetRoot(DEntry *dentry)
{
    if(!Lock()) return;
    root = dentry;
    UnLock();
}

DEntry *FileSystem::GetRoot()
{
    return root;
}

const char *FileSystem::GetLabel()
{
    return nullptr;
}

UUID FileSystem::GetUUID()
{
    return UUID::nil;
}

INode *FileSystem::ReadINode(ino_t number)
{
    return nullptr;
}

bool FileSystem::WriteINode(INode *inode)
{
    return false;
}

bool FileSystem::WriteSuperBlock()
{
    return false;
}

int FileSystem::Synchronize()
{
    return 0;
}

FileSystem::~FileSystem()
{
    remove(this);
    if(root) PutDEntry(root);
}
