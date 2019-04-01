#pragma once

#include <list.hpp>
#include <types.h>

class INode;
class StringBuilder;

class DEntry
{
    static void getPath(DEntry *dentry, StringBuilder &sb);
    const char *getFSLabel();
    int getFSId();
public:
    DEntry *Parent;
    char *Name;
    ::INode *INode;
    int ReferenceCount;

    DEntry(const char *name, DEntry *parent, class INode *inode);
    size_t GetFullPath(char *buffer, size_t bufferSize);
    ~DEntry();
};
