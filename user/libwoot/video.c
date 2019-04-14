#include <errno.h>
#include <syscalls/syscalls.h>
#include <woot/video.h>

int vidGetDisplayCount()
{
    return sysFBGetCount();
}

int vidGetDefaultDisplayId()
{
    return sysFBGetDefault();
}

int vidListDisplayIds(int *buf, unsigned bufSize)
{
    return sysFBListIds(buf, bufSize);
}

int vidGetDisplayName(int id, char *buf, unsigned bufSize)
{
    return sysFBGetName(id, buf, bufSize);
}

int vidOpenDisplay(int id)
{
    return sysFBOpen(id);
}

int vidCloseDisplay(int display)
{
    return sysFBClose(display);
}

int vidGetModeCount(int display)
{
    return sysFBGetModeCount(display);
}

int vidGetModeInfo(int display, int mode, vidModeInfo_t *modeInfo)
{
    return sysFBGetModeInfo(display, mode, modeInfo);
}

int vidSetMode(int display, int mode)
{
    return sysFBSetMode(display, mode);
}

int vidSetMode2(int display, int width, int height, int bpp, int refresh, int index)
{
    int modeCount = vidGetModeCount(display);
    vidModeInfo_t mi;
    for(int i = 0; i < modeCount; ++i)
    {
        if(vidGetModeInfo(display, i, &mi) < 0)
            continue;
        if(width >= 0 && mi.Width != width)
            continue;
        if(height >= 0 && mi.Height != height)
            continue;
        if(bpp >= 0 && mi.BitsPerPixel != bpp)
            continue;
        if(refresh >= 0 && mi.RefreshRate != refresh)
            continue;
        if(index >= 0 && index != (mi.Flags & VID_MI_FLAG_INDEX_COLOR))
            continue;
        return vidSetMode(display, i);
    }
    return -ENOENT;
}

void *vidMapPixels(int display, void *hint)
{
    return sysFBMapPixels(display, hint);
}

int vidGetCurrentMode(int display)
{
    return sysFBGetCurrentMode(display);
}
