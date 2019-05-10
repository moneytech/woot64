#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <woot/input.h>
#include <woot/ipc.h>
#include <woot/pixmap.h>
#include <woot/thread.h>
#include <woot/ui.h>
#include <woot/wm.h>

#include "font.hpp"

static pmPixMap *conPixMap = nullptr;
static unsigned conX = 0;
static unsigned conY = 0;
static unsigned conWidth = 80;
static unsigned conHeight = 25;
static unsigned conTabSize = 8;
static pmColor_t conTextColor = pmColorGray;
static pmColor_t conBackColor = pmColorBlue;
static char *conCmdBuf = nullptr;
static char *conCmdBuf2 = nullptr;
static char **conCmdArgs = nullptr;
static unsigned conCmdArgC = 0;
static unsigned conCmdBufSize = 1024;
static char *conCWD = nullptr;
static unsigned conCWDSize = 1024;
static wmWindow_t *conWindow = nullptr;
static rcRectangle_t conDirtyRect = rcRectangleEmpty;
static int conLastUpdate = 512;

static void updateConsole();

static inline void putChar(uint x, uint y, int chr)
{
    unsigned char *glyph = font[chr];
    for(int iy = 0; iy < FONT_SCANLINES; ++iy)
    {
        for(int ix = 0; ix < 8; ++ix)
            pmSetPixel(conPixMap, x + ix, y + iy, (*glyph << ix) & 0x80 ? conTextColor : conBackColor);
        ++glyph;
    }
    rcRectangle_t dirty = { (int)x, (int)y, FONT_BITS, FONT_SCANLINES };
    conDirtyRect = rcAddP(&conDirtyRect, &dirty);
    if(--conLastUpdate < 0)
        updateConsole();
}

static void putChar(int chr)
{
    int chrX = conX * FONT_BITS;
    int chrY = conY * FONT_SCANLINES;

    if(chr == '\n')
    {
        conX = 0;
        ++conY;
    }
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

void *operator new(unsigned long size)
{
    return calloc(1, size);
}

void *operator new[](unsigned long size)
{
    return calloc(1, size);
}

void operator delete(void *ptr)
{
    free(ptr);
}

void operator delete[](void *ptr)
{
    free(ptr);
}

extern "C" int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    wmInitialize();

    conWindow = wmCreateWindow(WM_CW_USEDEFAULT, WM_CW_USEDEFAULT, 500, 400, WM_CWF_DEFAULT);
    if(!conWindow)
    {
        fprintf(stderr, "[console] Couldn't create main window\n");
        return -errno;
    }

    conTextColor = pmColorGray;
    //conBackColor = pmColorBlue;

    uiControl_t *rootControl = wmGetRootControl(conWindow);
    wmSetWindowTitle(conWindow, "Console");
    uiControlSetBorderStyle(rootControl, UI_BORDER_NONE);
    uiControlSetBackColor(rootControl, conBackColor);
    uiControlRedraw(rootControl);

    conPixMap = uiControlGetPixMap(rootControl);
    conDirtyRect = conPixMap->Contents;
    conWidth = conPixMap->Contents.Width / FONT_BITS;
    conHeight = conPixMap->Contents.Height / FONT_SCANLINES;
    conCmdBuf = new char[conCmdBufSize];
    conCmdBuf2 = new char[conCmdBufSize];
    int maxCmdArgC = conCmdBufSize / 2;
    conCmdArgs = new char *[maxCmdArgC];
    conCWD = new char[conCWDSize];

    putStr("WOOT console.\n");
    updateConsole();

    ipcMessage_t msg;
    unsigned conCmdIdx;
    for(;;)
    {
        conCmdIdx = 0;
        memset(conCmdBuf, 0, conCmdBufSize);
        memset(conCmdArgs, 0, sizeof(char *) * maxCmdArgC);
        memset(conCWD, 0, conCWDSize);
        getcwd(conCWD, conCWDSize);
        if(conCWD[0])
        {
            char title[64];
            snprintf(title, sizeof(title), "Console - %s", conCWD);
            wmSetWindowTitle(conWindow, title);
            putStr(conCWD);
        }
        else
        {
            wmSetWindowTitle(conWindow, "Console");
            putChar('?');
        }
        putStr("> ");
        updateConsole();

        bool quit = false;
        for(;;)
        {
            int res = ipcGetMessage(&msg, -1);
            if(res < 0 || msg.Number == MSG_QUIT)
            {
                quit = true;
                break;
            }
            else if(msg.Number == MSG_WM_EVENT)
            {
                wmEvent_t *event = (wmEvent_t *)msg.Data;
                if(event->Type == WM_EVT_KEYBOARD && !(event->Keyboard.Flags & WM_EVT_KB_RELEASED))
                {
                    int chr = inpTranslateKey(event->Keyboard.Key, INP_MOD_NONE);
                    if(chr)
                    {
                        if(conCmdIdx > 0 && chr == '\b')
                        {
                            conCmdBuf[--conCmdIdx] = 0;
                            putChar(chr);
                        }
                        else if(chr != '\b' && conCmdIdx < (conCmdBufSize - 1))
                        {
                            if(chr != '\n')
                            {
                                conCmdBuf[conCmdIdx++] = chr;
                                conCmdBuf[conCmdIdx] = 0;
                            }
                            putChar(chr);
                        }
                        updateConsole();
                    }
                    if(chr == '\n')
                        break;
                }
            }
        }
        if(quit) break;

        if(!conCmdBuf[0])
            continue;

        memcpy(conCmdBuf2, conCmdBuf, conCmdBufSize);
        char *savePtr = nullptr;
        for(conCmdArgC = 0; conCmdArgC < maxCmdArgC; ++conCmdArgC)
        {
            char *arg = strtok_r(conCmdArgC ? NULL : conCmdBuf2, " \t", &savePtr);
            if(!arg) break;
            conCmdArgs[conCmdArgC] = arg;
        }

        if(!strcmp(conCmdArgs[0], "quit") || !strcmp(conCmdArgs[0], "exit"))
            break;
        else if(!strcmp(conCmdArgs[0], "pwd"))
        {
            putStr(conCWD);
            putStr("\n");
        }
        else if(!strcmp(conCmdArgs[0], "cd"))
        {
            if(!conCmdArgs[1])
                putStr("missing directory name\n");
            else
            {
                if(chdir(conCmdArgs[1]) < 0)
                    putStr("couldn't change directory\n");
            }
        }
        else if(!strcmp(conCmdArgs[0], "cat") || !strcmp(conCmdArgs[0], "type"))
        {
            if(!conCmdArgs[1])
                putStr("missing filename\n");
            else
            {
                FILE *f = fopen(conCmdArgs[1], "rb");
                if(!f) putStr("couldn't open file\n");
                else
                {
                    unsigned char buf[512];
                    int r = 0;
                    unsigned char lastChr = 0;
                    while((r = fread(buf, 1, sizeof(buf), f)) > 0)
                    {
                        for(int i = 0; i < r; ++i)
                        {
                            lastChr = buf[i];
                            putChar(lastChr);
                        }
                    }
                    fclose(f);
                    if(lastChr != '\n')
                        putStr("\n");
                }
            }
        }
        else
        {
            putStr("unknown command: '");
            putStr(conCmdArgs[0]);
            putStr("'\n");
        }
        updateConsole();
    }

    if(conCWD) delete[] conCWD;
    if(conCmdArgs) delete[] conCmdArgs;
    if(conCmdBuf2) delete[] conCmdBuf2;
    if(conCmdBuf) delete[] conCmdBuf;
    if(conWindow) wmDeleteWindow(conWindow);
    wmCleanup();
    return 0;
}
