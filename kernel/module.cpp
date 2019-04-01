#include <debug.hpp>
#include <elf.hpp>
#include <errno.h>
#include <misc.hpp>
#include <module.hpp>
#include <string.hpp>

List<Module *> Module::modules;
Mutex Module::listLock(false, "Module::listLock");

bool Module::lockList()
{
    return listLock.Acquire(1000, false);
}

void Module::unLockList()
{
    listLock.Release();
}

bool Module::append(Module *module)
{
    if(!lockList()) return false;
    modules.Append(module);
    unLockList();
    return true;
}

bool Module::remove(Module *module)
{
    if(!lockList()) return false;
    uint res = modules.Remove(module, nullptr, false);
    unLockList();
    return res != 0;
}

Module *Module::GetByName(const char *name)
{
    if(!lockList()) return nullptr;
    Module *res = nullptr;
    for(Module *module : modules)
    {
        if(!String::Compare(name, module->Name))
        {
            res = module;
            break;
        }
    }
    unLockList();
    return res;
}

int Module::Load(const char *filename)
{
    ELF *module = ELF::Load(filename, false, false, true);
    if(!module) return errno ? -errno : -EINVAL;
    int res = module->EntryPoint();
    if(IS_ERROR(res))
    {
        if(module->_module_fini)
            module->_module_fini();
        delete module;
    }
    return res;
}

Module::Module(const char *name, ProbeCallback probe, CleanupCallback clean) :
    Name(String::Duplicate(name)),
    CallbackProbe(probe),
    CallbackCleanup(clean)
{
    append(this);
}

int Module::Probe()
{
    return CallbackProbe ? CallbackProbe(this) : -ENOSYS;
}

int Module::Cleanup()
{
    return CallbackCleanup ? CallbackCleanup(this) : -ENOSYS;
}

Module::~Module()
{
    remove(this);
    if(CallbackCleanup)
        CallbackCleanup(this);
    if(Name) delete[] Name;
}
