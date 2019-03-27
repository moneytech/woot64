#include <multiboot.h>
#include <types.h>

extern "C" int kmain(multiboot_info_t *mbootInfo)
{
    asm("int3");
    return 0xABCD;
}

extern "C" void preInitialize(multiboot_info_t *mbootInfo)
{

}
