#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <woot/font.h>
#include <woot/ipc.h>
#include <woot/pixmap.h>
#include <woot/rectangle.h>
#include <woot/ui.h>
#include <woot/uibutton.h>
#include <woot/uilabel.h>
#include <woot/wm.h>

#include "window.hpp"

#undef min
#define min(x, y) ((x) < (y) ? (x) : (y))

int Window::ids = 0;
int Window::TitleBarHeight = 24;

void Window::titleBarButtonActivate(uiControl_t *sender)
{
    Window *wnd = (Window *)uiControlGetContext(sender);
    uiButton_t *btn = (uiButton_t *)sender;

    if(btn == wnd->closeButton)
    {
        wmEvent_t event;
        event.Type = WM_EVT_CLOSE;
        ipcSendMessage(wnd->GetOwner(), MSG_WM_EVENT, MSG_FLAG_NONE, &event, sizeof(event));
    }
}

Window::Window(int owner, int x, int y, unsigned w, unsigned h, unsigned flags, pmPixMap_t *dstPixMap, pmPixelFormat *pfOverride) :
    id(++ids), owner(owner), rect({ x, y, (int)w, (int)h }), flags(flags), shMemName(nullptr),
    pixels(nullptr), pixelsShMem(-ENOMEM), pixMap(nullptr), title(nullptr), active(true),
    dstPixMap(dstPixMap), titleBarPixMap(nullptr), titleBar(nullptr)
{
    char nameBuf[64];
    snprintf(nameBuf, sizeof(nameBuf), "window_%d_pixels", id);
    size_t pmPitch = pmFormatPixelsToBytesP(pfOverride ? pfOverride : &dstPixMap->Format, rect.Width);
    pixelsShMem = ipcCreateSharedMem(nameBuf, pmPitch * rect.Height);
    if(pixelsShMem >= 0)
    {
        pixels = ipcMapSharedMem(pixelsShMem, NULL, IPC_SHMEM_WRITE);
        if(pixels)
            pixMap = pmFromMemory(rect.Width, rect.Height, pmPitch, pfOverride ? pfOverride : &dstPixMap->Format, pixels, 0);
    }
    shMemName = strdup(nameBuf);

    if(flags & WM_CWF_TITLEBAR)
    {
        titleBarPixMap = pmCreate(w, TitleBarHeight, &dstPixMap->Format);
        if(titleBarPixMap)
            titleBar = uiControlCreate(nullptr, 0, titleBarPixMap, 0, 0, w, TitleBarHeight, nullptr);
    }

    if(titleBar)
    {
        pmColor_t defBg = wmGetColor(WM_COLOR_BACKGROUND);
        pmColor_t bg = wmGetColor(WM_COLOR_TITLE_BAR);
        pmColor_t fg = wmGetColor(WM_COLOR_TITLE_TEXT);
        fntFont_t *titleFont = wmGetFont(WM_FONT_TITLE_BAR);
        fntFont_t *symFont = wmGetFont(WM_FONT_UI_SYMBOLS);

        titleBarText = uiLabelCreate(titleBar, 1, 1, w - 2, TitleBarHeight - 2, "Window");
        rcRectangle_t lblRect = uiControlGetSize((uiControl_t *)titleBarText);
        int btnMargin = 4;
        int btnSize = lblRect.Height - btnMargin;

        // close button
        int closeX = lblRect.Width - btnSize - btnMargin / 2;
        if(flags & WM_CWF_CLOSEBUTTON)
        {
            closeButton = uiButtonCreate((uiControl_t *)titleBarText, closeX, (lblRect.Height - btnSize) / 2, btnSize, btnSize, "X");
            uiControlSetBackColor((uiControl_t *)closeButton, defBg);
            uiControlSetFont((uiControl_t *)closeButton, symFont);
            uiControlSetContext((uiControl_t *)closeButton, this);
            uiControlSetOnActivate((uiControl_t *)closeButton, titleBarButtonActivate);
            uiControlSetCanHaveFocus((uiControl_t *)closeButton, UI_FALSE);
        }
        else closeX = lblRect.Width + 1 - btnMargin / 2;

        int maxX = closeX - btnSize - 1;
        if(flags & WM_CWF_MAXIMIZEBUTTON)
        {
            maxButton = uiButtonCreate((uiControl_t *)titleBarText, maxX, (lblRect.Height - btnSize) / 2, btnSize, btnSize, "#");
            uiControlSetBackColor((uiControl_t *)maxButton, defBg);
            uiControlSetFont((uiControl_t *)maxButton, symFont);
            uiControlSetCanHaveFocus((uiControl_t *)maxButton, UI_FALSE);
        } else maxX = closeX - 1;

        int minX = maxX - btnSize;
        if(flags & WM_CWF_MINIMIZEBUTTON)
        {
            minButton = uiButtonCreate((uiControl_t *)titleBarText, minX, (lblRect.Height - btnSize) / 2, btnSize, btnSize, "_");
            uiControlSetBackColor((uiControl_t *)minButton, defBg);
            uiControlSetFont((uiControl_t *)minButton, symFont);
            uiControlSetCanHaveFocus((uiControl_t *)minButton, UI_FALSE);
        } else minX = maxX;

        if(flags & WM_CWF_SHOWICON)
        {
            progButton = uiButtonCreate((uiControl_t *)titleBarText, btnMargin / 2, (lblRect.Height - btnSize) / 2, btnSize, btnSize, "=");
            uiControlSetFont((uiControl_t *)progButton, symFont);
        }

        uiControlSetBackColor(titleBar, bg);
        uiControlSetBorderStyle(titleBar, UI_BORDER_RAISED);

        uiControlSetTextColor((uiControl_t *)titleBarText, fg);
        uiControlSetBackColor((uiControl_t *)titleBarText, bg);
        uiControlSetFont((uiControl_t *)titleBarText, titleFont);

        uiControlRedraw(titleBar, 0);
    }
}

int Window::GetId() const
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

rcRectangle_t Window::GetTitleRect() const
{
    if(!(flags & WM_CWF_TITLEBAR))
        return rcRectangleEmpty;
    rcRectangle_t r = rect;
    r.Height = TitleBarHeight;
    r.Y -= r.Height;
    return r;
}

rcRectangle_t Window::GetDragRect() const
{
    if(!(flags & WM_CWF_TITLEBAR))
        return rcRectangleEmpty;
    rcRectangle_t r = rect;
    r.Height = TitleBarHeight;
    r.Y -= r.Height;
#undef WINDOW_BUTTON_DRAG
#define WINDOW_BUTTON_DRAG 1 // set to 0 to disable dragging by titlebar buttons
#if !WINDOW_BUTTON_DRAG
    if(minButton)
    {
        int x; uiControlGetPosition((uiControl_t *)minButton, &x, NULL, 1);
        r.Width = min(x, r.Width);
    }
    if(maxButton)
    {
        int x; uiControlGetPosition((uiControl_t *)maxButton, &x, NULL, 1);
        r.Width = min(x, r.Width);
    }
    if(closeButton)
    {
        int x; uiControlGetPosition((uiControl_t *)closeButton, &x, NULL, 1);
        r.Width = min(x, r.Width);
    }
    if(progButton)
    {
        int x; uiControlGetPosition((uiControl_t *)progButton, &x, NULL, 1);
        rcRectangle_t size = uiControlGetSize((uiControl_t *)progButton);
        x += size.Width;
        r.X += x;
        r.Width -= x;
    }
#endif // !WINDOW_BUTTON_DRAG
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
    if(titleBarText)
    {
        uiControlSetText((uiControl_t *)titleBarText, this->title);
        uiControlRedraw(titleBar, 0);
    }
}

const char *Window::GetTitle() const
{
    return (const char *)title;
}

void Window::UpdateWindowGraphics(rcRectangle_t *dstDirtyRect)
{
    rcRectangle_t rc = GetRect();
    rcRectangle_t drc = GetDecoratedRect();
    rcRectangle_t is = rcIntersectP(dstDirtyRect, &drc);
    if(rcIsEmptyP(&is))
        return;

    if((flags & WM_CWF_TITLEBAR) && titleBar)
    {
        rcRectangle_t titleBar = { rc.X, rc.Y - TitleBarHeight, rc.Width, TitleBarHeight };
        is = rcIntersectP(dstDirtyRect, &titleBar);
        if(!rcIsEmptyP(&is))
        {
            pmPixMap_t *pm = uiControlGetPixMap(this->titleBar);
            pmBlit(dstPixMap, pm, is.X - titleBar.X, is.Y - titleBar.Y, is.X, is.Y, is.Width, is.Height);
        }
    }

    is = rcIntersectP(dstDirtyRect, &rc);
    if(rcIsEmptyP(&is))
        return;
    pmPixMap_t *pm = GetPixMap();
    int sx = is.X - rc.X;
    int sy = is.Y - rc.Y;

    if(flags & WM_CWF_USEALPHA)
        pmAlphaBlit(dstPixMap, pm, sx, sy, is.X, is.Y, is.Width, is.Height);
    else pmBlit(dstPixMap, pm, sx, sy, is.X, is.Y, is.Width, is.Height);
}

rcRectangle_t Window::SetActive(bool active)
{
    this->active = active;
    if(titleBar)
    {
        pmColor_t bg = wmGetColor(active ? WM_COLOR_TITLE_BAR : WM_COLOR_INACTIVE_TITLE_BAR);
        pmColor_t fg = wmGetColor(active ? WM_COLOR_TITLE_TEXT : WM_COLOR_INACTIVE_TITLE_TEXT);
        uiControlSetBackColor(titleBar, bg);
        uiControlSetBackColor((uiControl_t *)titleBarText, bg);
        uiControlSetTextColor((uiControl_t *)titleBarText, fg);
        uiControlRedraw((uiControl_t *)titleBar, 0);
    }
    rcRectangle_t titleBar = { rect.X, rect.Y - TitleBarHeight, rect.Width, TitleBarHeight };
    return titleBar;
}

bool Window::GetActive() const
{
    return active;
}

uiControl_t *Window::GetTitleControl() const
{
    return titleBar;
}

Window::~Window()
{
    if(titleBar) uiControlDelete(titleBar);
    if(titleBarPixMap) pmDelete(titleBarPixMap);
    if(shMemName) free(shMemName);
    if(pixMap) pmDelete(pixMap);
    if(pixels) ipcUnMapSharedMem(pixelsShMem, pixels);
    if(pixelsShMem) ipcCloseSharedMem(pixelsShMem);
    if(title) free(title);
}
