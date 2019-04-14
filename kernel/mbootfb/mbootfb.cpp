#include <errno.h>
#include <mbootfb.hpp>
#include <memory.hpp>
#include <multiboot.h>
#include <sysdefs.h>

extern "C" multiboot_info_t *__multibootInfo;
static multiboot_info_t *multibootInfo;

MultiBootFB::MultiBootFB()
{
    if(!multibootInfo) multibootInfo = (multiboot_info_t *)(KERNEL_BASE + (uintptr_t)__multibootInfo);
    mode.Width = multibootInfo->framebuffer_width;
    mode.Height = multibootInfo->framebuffer_height;
    mode.BitsPerPixel = multibootInfo->framebuffer_bpp;
    mode.RefreshRate = 0;
    mode.Pitch = multibootInfo->framebuffer_pitch;
    mode.Flags = multibootInfo->framebuffer_type == 0 ? FBMI_FLAG_INDEX_COLOR : 0;
    mode.AlphaBits = 0;
    mode.RedBits = multibootInfo->framebuffer_red_mask_size;
    mode.GreenBits = multibootInfo->framebuffer_green_mask_size;
    mode.BlueBits = multibootInfo->framebuffer_blue_mask_size;
    mode.AlphaShift = 0;
    mode.RedShift = multibootInfo->framebuffer_red_field_position;
    mode.GreenShift = multibootInfo->framebuffer_green_field_position;
    mode.BlueShift = multibootInfo->framebuffer_blue_field_position;
}

int MultiBootFB::GetModeCount()
{
    return 1;
}

int MultiBootFB::GetModeInfo(int mode, FrameBuffer::ModeInfo *info)
{
    if(mode != 0 || !info)
        return -EINVAL;
    Memory::Move(info, &this->mode, sizeof(ModeInfo));
    return ESUCCESS;
}

int MultiBootFB::SetMode(int mode)
{
    if(mode != 0)
        return -EINVAL;
    return ESUCCESS;
}

int MultiBootFB::GetCurrentMode()
{
    return 0;
}

uintptr_t MultiBootFB::GetBuffer()
{
    return multibootInfo->framebuffer_addr;
}

const char *MultiBootFB::GetName()
{
    return "Multiboot framebuffer";
}
