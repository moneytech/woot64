#include <character.hpp>
#include <dentry.hpp>
#include <errno.h>
#include <file.hpp>
#include <filesystem.hpp>
#include <inode.hpp>
#include <mutex.hpp>
#include <process.hpp>
#include <string.hpp>
#include <stringbuilder.hpp>
#include <sysdefs.h>
#include <tokenizer.hpp>
#include <volume.hpp>

File *File::open(::DEntry *parent, const char *name, int flags, mode_t createMode, bool followSymLinks)
{
    //printf("open %s\n", name);
    if(!parent || !parent->INode || !name)
        return nullptr;

    Tokenizer path(name, PATH_SEPARATORS, 0);

    ::DEntry *dentry = FileSystem::GetDEntry(parent);
    if(!dentry) return nullptr;
    for(Tokenizer::Token t : path.Tokens)
    {
        if(!String::Compare(".", t.String))
            continue;
        else if(!String::Compare("..", t.String))
        {
            if(dentry->Parent)
            {
                ::DEntry *nextDe = FileSystem::GetDEntry(dentry->Parent);
                FileSystem::PutDEntry(dentry);
                dentry = nextDe;
            }
            continue;
        }
        ::DEntry *nextDe = FileSystem::LookupDEntry(dentry, t.String);
        FileSystem::PutDEntry(dentry);
        if(!nextDe)
        {
            if(flags & O_CREAT)
            {
                parent->INode->Create(name, S_IFREG | createMode);
                return open(parent, name, flags & ~O_CREAT, 0, followSymLinks);
            }
            return nullptr;
        }
        dentry = nextDe;

        mode_t mode = dentry->INode->GetMode();
        if(S_ISLINK(mode) && followSymLinks)
        {
            char *linkPath = new char[MAX_PATH_LENGTH];
            int res = dentry->INode->GetLink(linkPath, MAX_PATH_LENGTH);
            FileSystem::PutDEntry(dentry);
            if(res < 0)
            {
                delete[] linkPath;
                return nullptr;
            }
            File *file = open(parent, linkPath, flags, createMode, followSymLinks);
            if(!file)
            {   // broken symlink
                delete[] linkPath;
                return nullptr;
            }
            dentry = FileSystem::GetDEntry(file->DEntry);
            if(dentry->Name) delete[] dentry->Name;
            dentry->Name = String::Duplicate(t.String);
            dentry->Parent = FileSystem::GetDEntry(parent);
            delete file;
            delete[] linkPath;
        }
    }
    if((flags & O_ACCMODE) != O_RDONLY && flags & O_TRUNC)
    {
        if(dentry->INode->Resize(0) != 0)
        {
            FileSystem::PutDEntry(dentry);
            return nullptr;
        }
    }
    mode_t mode = dentry->INode->GetMode();
    if((flags & O_DIRECTORY && !S_ISDIR(mode)))
    {
        FileSystem::PutDEntry(dentry);
        return nullptr;
    }

    File *file = new File(dentry, flags, mode);
    if(flags & O_APPEND)
        file->Position = file->GetSize();
    return file;
}

File::File(::DEntry *dentry, int flags, mode_t mode) :
    DEntry(dentry),
    Flags(flags),
    Position(0),
    Mode(mode)
{
}

int64_t File::getSize()
{
    return DEntry && DEntry->INode ? static_cast<int64_t>(DEntry->INode->GetSize()) : -EINVAL;
}

File *File::Open(::DEntry *parent, const char *name, int flags, mode_t mode, bool followSymLinks)
{
    //printf("open %s\n", name);
    if(!FileSystem::Lock())
        return nullptr;
    File *file = open(parent, name, flags, mode, followSymLinks);
    FileSystem::UnLock();
    return file;
}

File *File::Open(const char *name, int flags, mode_t mode, bool followSymLinks)
{
    if(!name || !String::Length(name))
        name = ".";
    name = reinterpret_cast<const char *>(String::TrimStart(const_cast<char *>(name), " \t"));
    Tokenizer path(name, PATH_SEPARATORS, 0);
    if(!path[0]) return nullptr;
    char *fsSep = path[0] ? String::Find(path[0], FS_SEPARATOR, false) : nullptr;
    if(fsSep) *fsSep = 0;

    if(!FileSystem::Lock())
        return nullptr;
    bool hasFs = fsSep;
    FileSystem *fs = nullptr;
    if(hasFs)
    {
        fs = FileSystem::GetByName(fsSep ? path[0] : "0");
        if(!fs)
        {
            FileSystem::UnLock();
            return nullptr;
        }
    }
    bool absolute = !hasFs && name[0] == '/';
    if(absolute)
    {
        ::DEntry *cd = Process::GetCurrentDir();
        if(!cd)
        {
            FileSystem::UnLock();
            return nullptr;
        }
        fs = cd->INode->FS;
    }
    ::DEntry *dentry = hasFs || absolute ? fs->GetRoot() : Process::GetCurrentDir();

    if(!dentry)
    {
        FileSystem::UnLock();
        return nullptr;
    }

    File *file = open(dentry, name + (hasFs ? path.Tokens[1].Offset : 0), flags, mode, followSymLinks);
    FileSystem::UnLock();
    return file;
}

int64_t File::GetSize()
{
    if(!FileSystem::Lock())
        return -EBUSY;
    int64_t size = getSize();
    FileSystem::UnLock();
    return size;
}

bool File::SetAccessTime(time_t time)
{
    if(!FileSystem::Lock())
        return -EBUSY;
    bool res = DEntry && DEntry->INode ? DEntry->INode->SetAccessTime(time) : -EINVAL;
    FileSystem::UnLock();
    return res;
}

bool File::SetModifyTime(time_t time)
{
    if(!FileSystem::Lock())
        return -EBUSY;
    bool res = DEntry && DEntry->INode ? DEntry->INode->SetModifyTime(time) : -EINVAL;
    FileSystem::UnLock();
    return res;
}

bool File::Create(const char *name, mode_t mode)
{
    if(!FileSystem::Lock())
        return false;
    bool res = DEntry && DEntry->INode ? DEntry->INode->Create(name, mode) : false;
    FileSystem::UnLock();
    return res;
}

int File::Remove(const char *name)
{
    if(!FileSystem::Lock())
        return false;
    int res = DEntry && DEntry->INode ? DEntry->INode->Remove(name) : -EINVAL;
    FileSystem::UnLock();
    return res;
}

int64_t File::Seek(int64_t offs, int loc)
{
    if(S_ISDIR(Mode) && (offs != 0 || loc != SEEK_SET))
        return -EISDIR;
    switch(loc)
    {
    case SEEK_SET:
        Position = offs;
        break;
    case SEEK_CUR:
        Position += offs;
        break;
    case SEEK_END:
        Position = getSize() - offs;
        break;
    default:
        break;
    }
    if(Position < 0) Position = 0;
    int64_t res = Position;
    return res;
}

int64_t File::Read(void *buffer, int64_t n)
{
    if(!DEntry) return -EINVAL;
    if(S_ISDIR(Mode))
        return -EISDIR;
    if((Flags & O_ACCMODE) == O_WRONLY)
        return -EINVAL;
    if(!FileSystem::Lock())
        return -EBUSY;
    int64_t res = DEntry->INode ? DEntry->INode->Read(buffer, Position, n) : -EINVAL;
    if(res > 0) Position += res;
    FileSystem::UnLock();
    return res;
}

int64_t File::Write(const void *buffer, int64_t n)
{
    if(!DEntry) return -EINVAL;
    if(S_ISDIR(Mode))
        return -EISDIR;
    if((Flags & O_ACCMODE) == O_RDONLY)
        return -EINVAL;
    if(!FileSystem::Lock())
        return -EBUSY;
    int64_t res = DEntry->INode ? DEntry->INode->Write(buffer, Position, n) : -EINVAL;
    if(res > 0) Position += res;
    FileSystem::UnLock();
    return res;
}

int64_t File::Rewind()
{
    return Seek(0, SEEK_SET);
}

DirectoryEntry *File::ReadDir()
{
    if(!DEntry)
        return nullptr;
    if(!(S_ISDIR(Mode)))
    {   // not a directory
        return nullptr;
    }
    if((Flags & O_ACCMODE) == O_WRONLY)
        return nullptr;
    if(!FileSystem::Lock())
        return nullptr;
    uint64_t newPos = Position;
    DirectoryEntry *res = DEntry->INode ? DEntry->INode->ReadDir(Position, &newPos) : nullptr;
    if(res) Position = newPos;
    FileSystem::UnLock();
    return res;
}

File::~File()
{
    //printf("close\n");
    FileSystem::PutDEntry(DEntry);
}
