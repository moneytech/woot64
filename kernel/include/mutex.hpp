#pragma once

#include <queue.hpp>
#include <types.h>

class Thread;

class Mutex
{
    bool Recursive;
    volatile int Count;
    volatile Thread *Owner;
    Queue<Thread *> *Waiters;
public:
    const char *Name;

    Mutex(bool recursive, const char *name);
    bool Acquire(uint timeout, bool tryAcquire = false);
    void Release();
    void Cancel(Thread *t);
    int GetCount() const;
    ~Mutex();
};
