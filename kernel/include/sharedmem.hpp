#pragma once

#include <list.hpp>
#include <mutex.hpp>
#include <namedobject.hpp>
#include <types.h>
#include <vector.hpp>

class Process;

class SharedMem
{
public:
    struct Mapping
    {
        Process *Owner;
        uintptr_t VA;

        bool operator ==(Mapping &b);
    };

private:
    Vector<uintptr_t> frames;
    List<Mapping> mappings;
    Mutex lock;
    size_t pageCount;

public:
    SharedMem(size_t size, bool cont);
    bool Lock();
    int Map(Process *proc, uintptr_t va, bool user, bool write);
    int UnMap(Process *proc, uintptr_t va);
    void UnLock();
    size_t GetSize() const;
    ~SharedMem();
};

class NamedSharedMem : public NamedObject, public SharedMem
{
public:
    NamedSharedMem(const char *name, size_t size, bool cont);
};
