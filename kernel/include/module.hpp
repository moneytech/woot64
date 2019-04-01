#pragma once

#include <list.hpp>
#include <mutex.hpp>
#include <types.h>

class ELF;

class Module
{
    static List<Module *> modules;
    static Mutex listLock;

    static bool lockList();
    static void unLockList();
    static bool append(Module *module);
    static bool remove(Module *module);
public:
    typedef int (*ProbeCallback)(Module *module);
    typedef int (*CleanupCallback)(Module *module);

    char *Name;
    ELF *Image;

    ProbeCallback CallbackProbe = nullptr;
    CleanupCallback CallbackCleanup = nullptr;

    static Module *GetByName(const char *name);
    static int Load(const char *filename);

    Module(const char *name, ProbeCallback probe, CleanupCallback clean);
    int Probe();
    int Cleanup();
    ~Module();
};
