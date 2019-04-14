#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define VID_MI_FLAG_INDEX_COLOR 1

// KERNEL_MATCH: syscalls.cpp struct vidModeInfo
typedef struct vidModeInfo
{
    int Width, Height;
    int BitsPerPixel;
    int RefreshRate;
    int Pitch;
    int Flags;
    int AlphaBits, RedBits, GreenBits, BlueBits;
    int AlphaShift, RedShift, GreenShift, BlueShift;
} vidModeInfo_t;

int vidGetDisplayCount();
int vidGetDefaultDisplayId();
int vidListDisplayIds(int *buf, unsigned bufSize);
int vidGetDisplayName(int id, char *buf, unsigned bufSize);
int vidOpenDisplay(int id);
int vidCloseDisplay(int display);
int vidGetModeCount(int display);
int vidGetModeInfo(int display, int mode, vidModeInfo_t *modeInfo);
int vidSetMode(int display, int mode);
int vidSetMode2(int display, int width, int height, int bpp, int refresh, int index);
void *vidMapPixels(int display, void *hint);
int vidGetCurrentMode(int display);

#ifdef __cplusplus
}
#endif // __cplusplus
