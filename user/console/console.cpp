#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <woot/ipc.h>
#include <woot/pixmap.h>
#include <woot/process.h>
#include <woot/rectangle.h>
#include <woot/thread.h>
#include <woot/wm.h>

#include "font.hpp"

static const char *progPrefix = "[console]";
static const pmColor_t conTextColor = pmColorLightGray;
static const pmColor_t conBackColor = pmColorBlue;
static wmWindow_t *conWindow;
static pmPixMap_t *conPixMap;
static rcRectangle_t conDirtyRect;
static int conLastUpdate;
static int conX, conY;
static int conWidth = 80;
static int conHeight = 25;
static int conTabSize = 4;
static bool quit = false;

static void updateConsole(void);

static inline void putChar(int x, int y, int chr)
{
    unsigned char *glyph = font[chr & 0xFF];
    for(int iy = 0; iy < FONT_SCANLINES; ++iy)
    {
        for(int ix = 0; ix < 8; ++ix)
            pmSetPixel(conPixMap, x + ix, y + iy, (*glyph << ix) & 0x80 ? conTextColor : conBackColor);
        ++glyph;
    }
    rcRectangle_t dirty = { x, y, FONT_BITS, FONT_SCANLINES };
    conDirtyRect = rcAddP(&conDirtyRect, &dirty);
    if(--conLastUpdate < 0)
        updateConsole();
}

static void putChar(int chr)
{
    int chrX = conX * FONT_BITS;
    int chrY = conY * FONT_SCANLINES;

    putChar(chrX, chrY, ' ');

    if(chr == '\n')
    {
        conX = 0;
        ++conY;
    }
    else if(chr == '\r')
        conX = 0;
    else if(chr == '\b')
    {
        if(conX > 0)
            --conX;
        else
        {
            if(conY > 0)
            {
                conX = conWidth - 1;
                --conY;
            }
        }
        chrX = conX * FONT_BITS;
        chrY = conY * FONT_SCANLINES;
        putChar(chrX, chrY, ' ');
    }
    else if(chr == '\t')
        conX = conTabSize * (conX + conTabSize - 1);
    else
    {   // regular character
        putChar(chrX, chrY, chr);
        ++conX;
    }

    if(conX >= conWidth)
    {   // end of line
        conX = 0;
        ++conY;
    }

    if(conY >= conHeight)
    {   // end of screen; scroll
        pmBlit(conPixMap, conPixMap, 0, FONT_SCANLINES, 0, 0, -1, conPixMap->Contents.Height - FONT_SCANLINES);
        pmFillRectangle(conPixMap, 0, conPixMap->Contents.Height - FONT_SCANLINES, conPixMap->Contents.Width, FONT_SCANLINES, conBackColor);
        conDirtyRect = conPixMap->Contents;
        conX = 0;
        --conY;
    }

    chrX = conX * FONT_BITS;
    chrY = conY * FONT_SCANLINES;
}

static void putStr(const char *str)
{
    while(*str)
        putChar(*str++);
}

static void updateConsole()
{
    if(rcIsEmptyP(&conDirtyRect))
        return;
    wmRedrawRect(conWindow, &conDirtyRect);
    conDirtyRect = rcRectangleEmpty;
    conLastUpdate = 512;
}

void ioThread(int fd)
{
    char buf[1024];
    for(int i = 0;; ++i)
    {
        ssize_t br = read(fd, buf, sizeof(buf));
        if(br < 0) return;
        for(int i = 0; i < br; ++i)
            putChar(buf[i]);
        if(br > 0) updateConsole();
    }
}

int execThread(uintptr_t arg)
{
    (void)arg;
    int pid = processCreate("/bin/sh");
    processWait(pid, -1);
    processDelete(pid);
    quit = true;
}

#define PIPE_READ_END   0
#define PIPE_WRITE_END  1

extern "C" int main(int argc, char *argv[])
{
    (void)argc, (void)argv;

    int pipeFDs[2];

    // redirect stdout and stderr
    pipe(pipeFDs);
    dup2(pipeFDs[PIPE_WRITE_END], 1);
    dup2(pipeFDs[PIPE_WRITE_END], 2);
    close(pipeFDs[PIPE_WRITE_END]);
    int stdouterrPipe = pipeFDs[PIPE_READ_END];

    // redirect stdin
    pipe(pipeFDs);
    dup2(pipeFDs[PIPE_READ_END], 0);
    close(pipeFDs[PIPE_READ_END]);
    int stdinPipe = pipeFDs[PIPE_WRITE_END];

    wmInitialize(WM_INITIALIZE_NONE);
    conWindow = wmCreateWindow(WM_CW_USEDEFAULT, WM_CW_USEDEFAULT, 640, 400, WM_CWF_DEFAULT);
    if(!conWindow)
    {
        fprintf(stderr, "%s Couldn't create the window\n", progPrefix);
        return -errno;
    }

    conPixMap = wmGetPixMap(conWindow);
    conDirtyRect = pmGetRectangle(conPixMap);
    conWidth = conDirtyRect.Width / FONT_BITS;
    conHeight = conDirtyRect.Height / FONT_SCANLINES;
    pmClear(conPixMap, conBackColor);

    wmSetWindowTitle(conWindow, "Console");

    int execThreadId = threadCreate("exec thread", reinterpret_cast<void *>(execThread), 0, nullptr);
    if(execThreadId < 0)
    {
        fprintf(stderr, "%s Couldn't create exec thread\n", progPrefix);
        wmDeleteWindow(conWindow);
        return -errno;
    }

    int ioThreadId = threadCreate("io thread", reinterpret_cast<void *>(ioThread), static_cast<uintptr_t>(stdouterrPipe), nullptr);
    if(ioThreadId < 0)
    {
        fprintf(stderr, "%s Couldn't create I/O thread\n", progPrefix);
        threadDelete(execThreadId);
        wmDeleteWindow(conWindow);
        return -errno;
    }

    threadResume(execThreadId);
    threadResume(ioThreadId);

    ipcMessage_t msg;
    while(ipcGetMessage(&msg, -1) >= 0)
    {
        ipcProcessMessage(&msg);
        if(msg.Number == MSG_QUIT)
            break;
        else if(msg.Number == MSG_WM_EVENT)
        {
            wmEvent_t *event = reinterpret_cast<wmEvent_t *>(msg.Data);
            wmProcessEvent(conWindow, event);
            if(event->Type == WM_EVT_CLOSE)
                break;
            else if(event->Type == WM_EVT_KEYBOARD)
            {
                char chr = static_cast<char>(event->Keyboard.Character);
                if(chr)
                {
                    write(1, &chr, 1);
                    write(stdinPipe, &chr, 1);
                }
            }
        }
        if(quit) break;
    }

    threadDelete(execThreadId);
    threadDelete(ioThreadId);
    close(stdouterrPipe);
    close(stdinPipe);

    wmDeleteWindow(conWindow);
    wmCleanup();
    return 0;
}
