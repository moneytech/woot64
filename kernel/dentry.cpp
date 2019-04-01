#include <debug.hpp>
#include <dentry.hpp>
#include <filesystem.hpp>
#include <inode.hpp>
#include <mutex.hpp>
#include <string.hpp>
#include <stringbuilder.hpp>
#include <sysdefs.h>

void DEntry::getPath(DEntry *dentry, StringBuilder &sb)
{
    if(!dentry) return;

    if(dentry->Parent)
    {
        if(dentry->Parent->Parent)
            getPath(dentry->Parent, sb);
        else
        {
            const char *label = dentry->getFSLabel();
            if(label) sb.WriteFmt("%s%c", label, FS_SEPARATOR);
            else sb.WriteFmt("%d", dentry->getFSId());
        }
        sb.WriteFmt("/%s", dentry->Name);
    }
    else
    {
        const char *label = dentry->getFSLabel();
        if(label) sb.WriteFmt("%s%c%s", label, FS_SEPARATOR, dentry->Name);
        else sb.WriteFmt("%d%c%s", dentry->getFSId(), FS_SEPARATOR, dentry->Name);
    }
}

const char *DEntry::getFSLabel()
{
    return INode->FS ? INode->FS->GetLabel() : nullptr;
}

int DEntry::getFSId()
{
    return INode->FS ? INode->FS->GetId() : -1;
}

DEntry::DEntry(const char *name, DEntry *parent, class INode *inode) :
    Parent(parent ? FileSystem::GetDEntry(parent) : nullptr),
    Name(String::Duplicate(name)),
    INode(inode),
    ReferenceCount(0)
{
    FileSystem::AddDEntry(this);
}

size_t DEntry::GetFullPath(char *buffer, size_t bufferSize)
{
    StringBuilder sb(buffer, bufferSize);
    getPath(this, sb);
    return String::Length(buffer);
}

DEntry::~DEntry()
{
    FileSystem::RemoveDEntry(this);
    if(Name) delete[] Name;
    if(INode) FileSystem::PutINode(INode);
    if(Parent) FileSystem::PutDEntry(Parent);
}
