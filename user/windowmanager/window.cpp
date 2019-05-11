#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <woot/font.h>
#include <woot/ipc.h>
#include <woot/pixmap.h>
#include <woot/rectangle.h>
#include <woot/wm.h>

#include "window.hpp"

#define align(val, alignment) ((alignment) * (((val) + ((alignment) - 1)) / (alignment)))

int Window::ids = 0;
int Window::TitleBarHeight = 24;
fntFont_t *Window::TitleFont = nullptr;

Window::Window(int owner, int x, int y, unsigned w, unsigned h, unsigned flags, pmPixelFormat_t *format) :
    id(++ids), owner(owner), rect({ x, y, (int)w, (int)h }), flags(flags), shMemName(nullptr),
    pixels(nullptr), pixelsShMem(-ENOMEM), pixMap(nullptr), title(nullptr)
{
    size_t pgSize = getpagesize();
    char nameBuf[64];
    snprintf(nameBuf, sizeof(nameBuf), "window_%d_pixels", id);
    size_t pmPitch = pmFormatPixelsToBytesP(format, rect.Width);
    pixelsShMem = ipcCreateSharedMem(nameBuf, align(pmPitch * rect.Height, pgSize));
    if(pixelsShMem >= 0)
    {
        pixels = ipcMapSharedMem(pixelsShMem, NULL, IPC_SHMEM_WRITE);
        if(pixels)
            pixMap = pmFromMemory(rect.Width, rect.Height, pmPitch, format, pixels, 0);
    }
    shMemName = strdup(nameBuf);
}

int Window::GetID() const
{
    return id;
}

int Window::GetOwner() const
{
    return owner;
}

pmPixelFormat_t Window::GetPixelFormat() const
{
    return pixMap ? pixMap->Format : pmFormatInvalid;
}

const char *Window::GetShMemName() const
{
    return shMemName;
}

rcRectangle_t Window::GetRect() const
{
    return rect;
}

rcRectangle_t Window::GetDecoratedRect() const
{
    rcRectangle_t r = rect;
    if(flags & WM_CWF_TITLEBAR)
    {
        r.Y -= TitleBarHeight;
        r.Height += TitleBarHeight;
    }
    return r;
}

rcRectangle_t Window::GetDragRect() const
{
    if(!(flags & WM_CWF_TITLEBAR))
        return rcRectangleEmpty;
    rcRectangle_t r = rect;
    r.Height = TitleBarHeight;
    r.Y -= r.Height;
    return r;
}

pmPixMap_t *Window::GetPixMap() const
{
    return pixMap;
}

unsigned Window::GetFlags() const
{
    return flags;
}

void Window::SetPosition(int x, int y)
{
    rcRectangle_t drc = GetDecoratedRect();
    int dx = drc.X - rect.X;
    int dy = drc.Y - rect.Y;
    rect.X = x - dx;
    rect.Y = y - dy;
}

void Window::SetTitle(const char *title)
{
    if(!title) return;
    if(this->title) free(this->title);
    this->title = strdup(title);
}

const char *Window::GetTitle() const
{
    return (const char *)title;
}

void Window::UpdateWindowGraphics(pmPixMap_t *dst, rcRectangle_t *dstDirtyRect)
{
    rcRectangle_t rc = GetRect();
    rcRectangle_t drc = GetDecoratedRect();
    rcRectangle_t is = rcIntersectP(dstDirtyRect, &drc);
    if(rcIsEmptyP(&is))
        return;

    if(flags & WM_CWF_TITLEBAR)
    {
        rcRectangle_t titleBar = { rc.X, rc.Y - TitleBarHeight, rc.Width, TitleBarHeight };
        is = rcIntersectP(dstDirtyRect, &titleBar);
        if(!rcIsEmptyP(&is))
        {
            pmFillRectangle(dst, is.X, is.Y, is.Width, is.Height, pmColorBlue);
            if(title && TitleFont)
            {
                int titleHeight = fntGetPixelHeight(TitleFont);
                int titleWidth = fntMeasureString(TitleFont, title);
                int cx = (titleBar.Width - titleWidth) / 2;
                int cy = (titleBar.Height - titleHeight) / 2;
                fntDrawString(TitleFont, dst, titleBar.X + cx, titleBar.Y + cy, title, pmColorWhite);
            }
            pmDrawFrameRect(dst, &titleBar, 0);
            //pmRectangleRect(dst, &titleBar, pmColorWhite);
        }
    }

    is = rcIntersectP(dstDirtyRect, &rc);
    if(rcIsEmptyP(&is))
        return;
    pmPixMap_t *pm = GetPixMap();
    int sx = is.X - rc.X;
    int sy = is.Y - rc.Y;

    if(flags & WM_CWF_USEALPHA)
        pmAlphaBlit(dst, pm, sx, sy, is.X, is.Y, is.Width, is.Height);
    else pmBlit(dst, pm, sx, sy, is.X, is.Y, is.Width, is.Height);
    //else pmRectangle(dst, is.X, is.Y, is.Width, is.Height, pmColorMagenta);
}

Window::~Window()
{
    if(shMemName) free(shMemName);
    if(pixMap) pmDelete(pixMap);
    if(pixels) ipcUnMapSharedMem(pixelsShMem, pixels);
    if(pixelsShMem) ipcCloseSharedMem(pixelsShMem);
    if(title) free(title);
}
