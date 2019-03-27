#include <cpu.hpp>
#include <multiboot.h>
#include <paging.hpp>
#include <types.h>

extern "C" int kmain(multiboot_info_t *mbootInfo)
{
    return 0xABCD;
}

extern "C" void preInitialize(multiboot_info_t *mbootInfo)
{
    Paging::Initialize(mbootInfo);
}
