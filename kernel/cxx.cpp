#include <types.h>

extern "C" uintptr_t __dso_handle;
uintptr_t __dso_handle = 0;

typedef void (*atexitHandler)(void *ptr, void *dso);
class atexitTableEntry
{
public:
    atexitHandler handler;
    void *ptr, *dso;
    bool handled;
    atexitTableEntry() : handler(nullptr), ptr(nullptr), dso(nullptr), handled(false) {}
    atexitTableEntry(atexitHandler handler, void *ptr, void *dso) :
        handler(handler), ptr(ptr), dso(dso), handled(false)
    {
    }
};

#define MAX_ATEXIT_HANDLERS 4096
static atexitTableEntry atexitTable[MAX_ATEXIT_HANDLERS];
static int atexitHandlers = 0;

extern "C" void __cxa_atexit(atexitHandler handler, void *ptr, void *dso)
{
    if(atexitHandlers >= MAX_ATEXIT_HANDLERS)
        return;
    atexitTable[atexitHandlers++] = atexitTableEntry(handler, ptr, dso);
}

extern "C" void __cxa_finalize(atexitHandler handler)
{
    for(int i = atexitHandlers - 1; i > 0; --i)
    {
        atexitTableEntry *entry = atexitTable + i;
        if(entry->handled || !entry->handler)
            continue;
        if(!handler || (handler && entry->handler))
        {
            entry->handler(entry->ptr, entry->dso);
            entry->handled = true;
        }
    }
}

void *operator new(size_t size)
{
    return nullptr;
}

void *operator new[](size_t size)
{
    return nullptr;
}

// placement new
void *operator new(size_t size, void *ptr)
{
    return ptr;
}

void operator delete(void *ptr)
{
}

void operator delete[](void *ptr)
{
}
