#include <errno.h>
#include <framebuffer.hpp>
#include <string.hpp>
#include <stringbuilder.hpp>

Sequencer<int> FrameBuffer::ids(0);
List<FrameBuffer *> FrameBuffer::fbs;
Mutex FrameBuffer::listLock(false, "FrameBuffer::listLock");

bool FrameBuffer::lockList()
{
    return listLock.Acquire(1000, false);
}

void FrameBuffer::unLockList()
{
    listLock.Release();
}

bool FrameBuffer::append(FrameBuffer *fb)
{
    if(!lockList()) return false;
    fbs.Append(fb);
    unLockList();
    return true;
}

bool FrameBuffer::remove(FrameBuffer *fb)
{
    if(!lockList()) return false;
    uint res = fbs.Remove(fb, nullptr, false);
    unLockList();
    return res != 0;
}

FrameBuffer::FrameBuffer() :
    id(ids.GetNext())
{
}

FrameBuffer *FrameBuffer::GetById(int id)
{
    if(!lockList()) return nullptr;
    FrameBuffer *res = nullptr;
    for(FrameBuffer *fb : fbs)
    {
        if(fb->id == id)
        {
            res = fb;
            break;
        }
    }
    unLockList();
    return res;
}

int FrameBuffer::FindMode(int width, int height, int bpp, int refresh, bool indexed)
{
    int modeCount = GetModeCount();
    ModeInfo mi;
    for(int i = 0; i < modeCount; ++i)
    {
        if(GetModeInfo(i, &mi) < 0)
            continue;
        if(width >= 0 && mi.Width != width)
            continue;
        if(height >= 0 && mi.Height != height)
            continue;
        if(bpp >= 0 && mi.BitsPerPixel != bpp)
            continue;
        if(refresh >= 0 && mi.RefreshRate != refresh)
            continue;
        if(indexed != (mi.Flags & FBMI_FLAG_INDEX_COLOR))
            continue;
        return i;
    }
    return -ENOENT;
}

int FrameBuffer::GetModeCount()
{
    return 0;
}

int FrameBuffer::GetModeInfo(int mode, FrameBuffer::ModeInfo *info)
{
    return -ENOSYS;
}

int FrameBuffer::SetMode(int mode)
{
    return -ENOSYS;
}

int FrameBuffer::GetCurrentMode()
{
    return -ENOSYS;
}

uintptr_t FrameBuffer::GetBuffer()
{
    return ~0;
}

FrameBuffer::~FrameBuffer()
{
}
