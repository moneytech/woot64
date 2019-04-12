#include <errno.h>
#include <mbootfb.hpp>
#include <module.hpp>
#include <multiboot.h>

extern "C" multiboot_info_t *__multibootInfo;

static MultiBootFB *fb = nullptr;

static int Probe(Module *module);
static int Cleanup(Module *module);

extern "C" int _module_start()
{
    new Module("mbootfb", Probe, Cleanup);
    return ESUCCESS;
}

static int Probe(Module *module)
{
    if(!__multibootInfo)
        return -ENODEV;
    if(!fb) fb = new MultiBootFB();
    return ESUCCESS;
}

static int Cleanup(Module *module)
{
    if(fb)
    {
        delete fb;
        fb = nullptr;
    }
    return ESUCCESS;
}
