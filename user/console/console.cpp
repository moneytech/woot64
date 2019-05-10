#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <woot/ipc.h>
#include <woot/pixmap.h>
#include <woot/ui.h>
#include <woot/wm.h>

#include "font.hpp"

static pmPixMap *conPixMap = nullptr;
static int conX = 0;
static int conY = 0;
static int conWidth = 80;
static int conHeight = 25;
static int conTabSize = 8;
static pmColor_t conTextColor = pmColorGray;
static pmColor_t conBackColor = pmColorBlue;
static char *conCmd = nullptr;

static inline void putChar(uint x, uint y, int chr)
{
    unsigned char *glyph = font[chr];
    for(int iy = 0; iy < FONT_SCANLINES; ++iy)
    {
        for(int ix = 0; ix < 8; ++ix)
        {
            if((*glyph << ix) & 0x80)
                pmSetPixel(conPixMap, x + ix, y + iy, conTextColor);
        }
        ++glyph;
    }
    pmInvalidate(conPixMap, x, y, FONT_BITS, FONT_SCANLINES);
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
        pmInvalidateWhole(conPixMap);
        conX = 0;
        --conY;
    }
}

static void putStr(const char *str)
{
    while(*str)
        putChar(*str++);
}

extern "C" int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    wmInitialize();

    wmWindow_t *window = wmCreateWindow(WM_CW_USEDEFAULT, WM_CW_USEDEFAULT, 500, 400, WM_CWF_DEFAULT);
    if(!window)
    {
        fprintf(stderr, "[console] Couldn't create main window\n");
        return -errno;
    }

    uiControl_t *rootControl = wmGetRootControl(window);
    wmSetWindowTitle(window, "Console");
    uiControlSetBorderStyle(rootControl, UI_BORDER_NONE);
    uiControlSetBackColor(rootControl, conBackColor);
    uiControlRedraw(rootControl);

    conTextColor = pmColorGray;

    conPixMap = uiControlGetPixMap(rootControl);
    conWidth = conPixMap->Contents.Width + FONT_BITS;
    conHeight = conPixMap->Contents.Height + FONT_SCANLINES;

    putStr("Trolololo!\n");
    //pmRectangleRect(conPixMap, &conPixMap->Dirty, pmColorMagenta);
    wmRedrawWindow(window);

    ipcMessage_t msg;
    for(;;)
    {
        bool quit = false;
        for(;;)
        {
            int res = ipcGetMessage(&msg, -1);
            if(res < 0 || msg.Number == MSG_QUIT)
            {
                quit = true;
                break;
            }

        }
        if(quit) break;


    }

    wmCleanup();
    return 0;
}
