#pragma once

#include <list.hpp>
#include <mutex.hpp>
#include <sequencer.hpp>
#include <types.h>
#include <uuid.hpp>

#define MAX_FS_LABEL_LENGTH 128
#define MAX_PATH_LENGTH     4095
#define FS_SEPARATOR        '~'
#define PATH_SEPARATORS     "/\\"

class DEntry;
class FileSystemType;
class INode;
class Volume;

class FileSystem
{
    static Sequencer<int> ids;
    static Mutex mutex;
    static List<DEntry *> dentryCache;
    static List<INode *> inodeCache;
    static List<FileSystem *> fileSystems;
    static Mutex listLock;

    DEntry *root = nullptr;

    static bool lockList();
    static void unLockList();
    static bool append(FileSystem *fs);
    static bool remove(FileSystem *fs);
protected:
    int id;
    Volume *volume;
    FileSystemType *type;

    FileSystem(Volume *volume, FileSystemType *type);
public:
    static void Initialize();
    static void Cleanup();
    static bool Lock();
    static void UnLock();
    static int DetectAll();
    static void AddDEntry(DEntry *dentry);
    static void RemoveDEntry(DEntry *dentry);
    static FileSystem *GetByName(const char *name);
    static void SynchronizeAll();

    static void PutINode(INode *inode);
    static DEntry *LookupDEntry(DEntry *parent, const char *name);
    static DEntry *GetDEntry(DEntry *dentry);
    static void PutDEntry(DEntry *dentry);

    INode *GetINode(ino_t number);
    int GetId();
    void SetRoot(DEntry *dentry);
    DEntry *GetRoot();

    virtual const char *GetLabel();
    virtual UUID GetUUID();
    virtual INode *ReadINode(ino_t number);
    virtual bool WriteINode(INode *inode);
    virtual bool WriteSuperBlock();
    virtual int Synchronize();

    virtual ~FileSystem();
};
