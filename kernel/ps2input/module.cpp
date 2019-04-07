#include <debug.hpp>
#include <errno.h>
#include <module.hpp>
#include <ps2kbd.hpp>
#include <ps2mouse.hpp>

static Module *mod;
static PS2Keyboard *kbd;
static PS2Mouse *mouse;

static int Probe(Module *module);
static int Cleanup(Module *module);

extern "C" int _module_start()
{
    mod = new Module("ps2input", Probe, Cleanup);
    return ESUCCESS;
}

static int Probe(Module *module)
{
    // TODO: add detection code
    if(!kbd) kbd = new PS2Keyboard();
    if(!mouse) mouse = new PS2Mouse();
    return ESUCCESS;
}

static int Cleanup(Module *module)
{
    if(kbd) delete kbd;
    if(mouse) delete mouse;
    return ESUCCESS;
}
