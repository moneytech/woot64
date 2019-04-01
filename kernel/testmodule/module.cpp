#include <debug.hpp>
#include <errno.h>

extern "C" int _module_init()
{
    DEBUG("[testmodule:module] _module_init() = %p\n", _module_init);
    errno = 0;
    return 123;
}

extern "C" void _module_fini()
{
}
