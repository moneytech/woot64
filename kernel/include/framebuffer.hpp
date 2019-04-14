#pragma once

#include <list.hpp>
#include <mutex.hpp>
#include <sequencer.hpp>
#include <types.h>

#define FBMI_FLAG_INDEX_COLOR  1

class Process;

class FrameBuffer
{
    static Sequencer<int> ids;
    static List<FrameBuffer *> fbs;
    static Mutex listLock;

    static bool lockList();
    static void unLockList();
    static bool append(FrameBuffer *fb);
    static bool remove(FrameBuffer *fb);
protected:
    int id;
    Process *owner;

    FrameBuffer();
public:
    struct ModeInfo
    {
        int Width, Height;
        int BitsPerPixel;
        int RefreshRate;
        int Pitch;
        int Flags;
        int AlphaBits, RedBits, GreenBits, BlueBits;
        int AlphaShift, RedShift, GreenShift, BlueShift;
    };

    static size_t GetCount();
    static FrameBuffer *GetById(int id);
    static int GetDefaultId();
    static int ListIds(int *buf, size_t bufSize);

    int FindMode(int width, int height, int bpp, int refresh, bool indexed); // -1 in any argument means don't care

    virtual int Open();
    virtual int Close();
    virtual int GetModeCount();
    virtual int GetModeInfo(int mode, ModeInfo *info);
    virtual int SetMode(int mode);
    virtual int GetCurrentMode();
    virtual uintptr_t GetBuffer(); // returns physical address
    virtual const char *GetName();
    virtual ~FrameBuffer();
};
