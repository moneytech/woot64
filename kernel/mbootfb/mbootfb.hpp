#pragma once

#include <framebuffer.hpp>
#include <types.h>

class MultiBootFB : public FrameBuffer
{
    ModeInfo mode;
public:
    MultiBootFB();
    virtual int GetModeCount();
    virtual int GetModeInfo(int mode, ModeInfo *info);
    virtual int SetMode(int mode);
    virtual int GetCurrentMode();
    virtual uintptr_t GetBuffer(); // returns physical address
    virtual const char *GetName();
};
