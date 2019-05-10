#include <errno.h>
#include <png.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <woot/font.h>
#include <woot/input.h>
#include <woot/ipc.h>
#include <woot/pixmap.h>
#include <woot/rpc.h>
#include <woot/thread.h>
#include <woot/video.h>
#include <woot/wm.h>

#include "vector.hpp"
#include "window.hpp"

union wmCreateWindowResp
{
    struct
    {
        int id;
        pmPixelFormat_t pixelFormat;
        char shMemName[0];
    };
    char Data[MSG_RPC_RESP_PAYLOAD_SIZE];
};

struct wmSetWindowPosArgs
{
    int windowId;
    int x, y;
};

struct wmSetWindowTitleArgs
{
    int windowId;
    const char title[0];
};

typedef Vector<Window *> Windows;
static Window *getWindowById(Windows *windows, int id);
static void moveWindow(rcRectangle_t *dirtyRect, Window *window, int x, int y);
static void updateRect(pmPixMap_t *fb, pmPixMap_t *bb, Windows *windows, rcRectangle_t *rect);

extern "C" int main(int argc, char *argv[])
{
    int screenWidth = 640;
    int screenHeight = 480;
    int screenDepth = 32;

    if(argc > 1) screenWidth = atoi(argv[1]);
    if(argc > 2) screenHeight = atoi(argv[2]);
    if(argc > 3) screenDepth = atoi(argv[3]);

    setbuf(stdout, NULL);

    int currentPId = getpid();
    printf("[windowmanager] Started window manager (pid: %d)\n", currentPId);

    int defaultDisplayId = vidGetDefaultDisplayId();
    if(defaultDisplayId < 0)
    {
        printf("[windowmanager] Couldn't get default display id\n");
        return -errno;
    }
    int display = vidOpenDisplay(defaultDisplayId);
    if(display < 0)
    {
        printf("[windowmanager] Couldn't open default display\n");
        return -errno;
    }

    printf("[windowmanager] Trying %dx%dx%d video mode\n", screenWidth, screenHeight, screenDepth);
    if(vidSetMode2(display, screenWidth, screenHeight, screenDepth, -1, -1) < 0)
    {
        printf("[windowmanager] Couldn't set video mode\n");
        return -errno;
    }

    int currMode = vidGetCurrentMode(display);
    if(currMode < 0)
    {
        printf("[windowmanager] Couldn't get current video mode number\n");
        return -errno;
    }

    vidModeInfo_t mi;
    if(vidGetModeInfo(display, currMode, &mi) < 0)
    {
        printf("[windowmanager] Couldn't get current video mode information\n");
        return -errno;
    }

    void *pixels = vidMapPixels(display, NULL);
    if(pixels == (void *)-1)
    {
        printf("[windowmanager] Couldn't get frame buffer data\n");
        return -errno;
    }

    fntFont_t *defaultFont = fntLoad("/default.ttf");
    fntFont_t *titleFont = fntLoad("/title.ttf");
    if(!defaultFont || !titleFont)
    {
        if(defaultFont) fntDelete(defaultFont);
        if(titleFont) fntDelete(titleFont);
        printf("[windowmanager] Couldn't load required font\n");
        return -errno;
    }
    fntSetPointSize(defaultFont, 12, 96);
    fntSetPointSize(titleFont, 12, 96);
    Window::TitleFont = titleFont;

    pmPixelFormat_t fbFormat = pmFormatFromModeInfo(&mi);

    pmPixMap_t *fbPixMap = pmFromMemory(mi.Width, mi.Height, mi.Pitch, &fbFormat, pixels, 0);
    pmPixMap_t *bbPixMap = pmFromPixMap(fbPixMap, &fbFormat); // create back buffer

    int cursorHotX = 0;
    int cursorHotY = 0;

    ipcSendMessage(0, MSG_ACQUIRE_KEYBOARD, MSG_FLAG_NONE, NULL, 0);
    ipcSendMessage(0, MSG_ACQUIRE_MOUSE, MSG_FLAG_NONE, NULL, 0);

    Vector<Window *> windows;

    threadDaemonize();

    ipcMessage_t msg;
    int mouseX = screenWidth / 2, mouseY = screenHeight / 2;

    // create desktop window
    Window *desktopWnd = new Window(currentPId, 0, 0, bbPixMap->Contents.Width, bbPixMap->Contents.Height, WM_CWF_NONE, &bbPixMap->Format);
    windows.Prepend(desktopWnd);
    pmPixMap_t *logo = pmLoadPNG("/logo.png");
    if(logo && desktopWnd)
    {
        pmPixMap_t *pm = desktopWnd->GetPixMap();
        pmClear(pm, pmColorFromRGB(24, 64, 96));
        pmAlphaBlit(pm, logo, 0, 0,
                    (bbPixMap->Contents.Width - logo->Contents.Width) / 2,
                    (bbPixMap->Contents.Height - logo->Contents.Height) / 2,
                    logo->Contents.Width, logo->Contents.Height);
        pmDelete(logo);
    }

    // create mouse cursor window
    Window *mouseWnd = nullptr;
    pmPixMap_t *cursor = pmLoadCUR("/normal.cur", 0, &cursorHotX, &cursorHotY);
    if(cursor)
    {
        mouseWnd = new Window(currentPId, mouseX - cursorHotX, mouseY - cursorHotY,
                              cursor->Contents.Width, cursor->Contents.Height,
                              WM_CWF_USEALPHA, &cursor->Format);
        pmBlit(mouseWnd->GetPixMap(), cursor, 0, 0, 0, 0, -1, -1);
        pmDelete(cursor);
        windows.Append(mouseWnd);
    }

    updateRect(fbPixMap, bbPixMap, &windows, &bbPixMap->Contents);
    for(int i = 0;; ++i)
    {
        rcRectangle_t dirtyRect = rcRectangleEmpty;
        ipcGetMessage(&msg, -1);

        do
        {
            ipcProcessMessage(&msg);
            if(msg.Number == MSG_QUIT)
                break;
            else if(msg.Number == MSG_KEYBOARD_EVENT)
            {
                inpKeyboardEvent_t *kbdEv = (inpKeyboardEvent_t *)msg.Data;
                printf("[usertest] key: %d %s\n", kbdEv->Key, kbdEv->Flags & INP_KBD_EVENT_FLAG_RELEASE ? "released" : "pressed");
            }
            else if(msg.Number == MSG_MOUSE_EVENT)
            {
                inpMouseEvent_t *mouseEv = (inpMouseEvent_t *)msg.Data;

                // mouse acceleration
                float mouseSpeed = sqrt(mouseEv->Delta[0] * mouseEv->Delta[0] + mouseEv->Delta[1] * mouseEv->Delta[1]);
                if(mouseSpeed > 4)
                {
                    mouseEv->Delta[0] *= 1.5;
                    mouseEv->Delta[1] *= 1.5;
                }

                mouseX += mouseEv->Delta[0];
                mouseY += mouseEv->Delta[1];

                if(mouseX < 0) mouseX = 0;
                else if(mouseX >= screenWidth)
                    mouseX = screenWidth - 1;
                if(mouseY < 0) mouseY = 0;
                else if(mouseY >= screenHeight)
                    mouseY = screenHeight - 1;

                if(mouseWnd)
                    moveWindow(&dirtyRect, mouseWnd, mouseX - cursorHotX, mouseY - cursorHotY);

                for(Window *wnd : windows)
                {
                    if(wnd == mouseWnd || wnd == desktopWnd)
                        continue;

                    rcRectangle_t rect = wnd->GetRect();
                    if(rcContainsPointP(&rect, mouseX, mouseY))
                    {
                        wmEvent_t event;
                        memset(&event, 0, sizeof(event));
                        event.Type = WM_EVT_MOUSE;
                        event.WindowId = wnd->GetID();
                        event.Mouse.Coords[0] = mouseX - rect.X;
                        event.Mouse.Coords[1] = mouseY - rect.Y;
                        for(int i = 0; i < WM_EVT_MOUSE_AXES && i < INP_MAX_MOUSE_AXES; ++i)
                            event.Mouse.Delta[i] = mouseEv->Delta[i];
                        event.Mouse.ButtonsPressed = mouseEv->ButtonsPressed;
                        event.Mouse.ButtonsHeld = mouseEv->ButtonsHeld;
                        event.Mouse.ButtonsReleased = mouseEv->ButtonsReleased;
                        ipcSendMessage(wnd->GetOwner(), MSG_WM_EVENT, MSG_FLAG_NONE, &event, sizeof(event));
                        break;
                    }
                }
            }
            else if(msg.Number == MSG_RPC_REQUEST)
            {
                char *req = (char *)msg.Data;
                char *args = (char *)(req + strlen(req) + 1);
                if(!strcmp(req, "wmCreateWindow"))
                {
                    int x = *(int *)(args);
                    int y = *(int *)(args + 4);
                    unsigned w = *(unsigned *)(args + 8);
                    unsigned h = *(unsigned *)(args + 12);
                    unsigned flags = *(unsigned *)(args + 16);

                    if(x == WM_CW_USEDEFAULT) x = (bbPixMap->Contents.Width - w) / 2;
                    if(y == WM_CW_USEDEFAULT) y = (bbPixMap->Contents.Height - h) / 2;

                    //printf("[windowmanager] wmCreateWindow(%d, %d, %d, %d, %#.8x)\n", x, y, w, h, flags);
                    Window *wnd = new Window(msg.Source, x, y, w, h, flags, &fbFormat);
                    dirtyRect = rcAdd(dirtyRect, wnd->GetDecoratedRect());

                    // add new window before mouse cursor window (if possible)
                    int mouseWndIdx = windows.Size();
                    for(int i = 0; i < mouseWndIdx; ++i)
                    {
                        if(windows.Get(i) == mouseWnd)
                        {
                            mouseWndIdx = i;
                            break;
                        }
                    }
                    windows.InsertBefore(mouseWndIdx, wnd);

                    wmCreateWindowResp response;

                    response.id = wnd->GetID();
                    response.pixelFormat = wnd->GetPixelFormat();
                    snprintf(response.shMemName, MSG_RPC_RESP_PAYLOAD_SIZE - offsetof(wmCreateWindowResp, shMemName), "%s", wnd->GetShMemName());

                    rpcIPCReturn(msg.Source, msg.ID, &response, sizeof(response));
                }
                else if(!strcmp(req, "wmDeleteWindow"))
                {
                    int id = *(int *)args;
                    Window *wnd = getWindowById(&windows, id);
                    if(wnd)
                    {
                        windows.RemoveOne(wnd);
                        delete wnd;
                    }
                    rpcIPCReturn(msg.Source, msg.ID, NULL, 0);
                }
                else if(!strcmp(req, "wmRedrawWindow"))
                {
                    int window = *(int *)(args);
                    Window *wnd = getWindowById(&windows, window);
                    rcRectangle rect = wnd ? wnd->GetDecoratedRect() : rcRectangleEmpty;
                    dirtyRect = rcAddP(&dirtyRect, &rect);
                    rpcIPCReturn(msg.Source, msg.ID, NULL, 0);
                }
                else if(!strcmp(req, "wmSetWindowPos"))
                {
                    wmSetWindowPosArgs *swpa = (decltype(swpa))(args);
                    Window *wnd = getWindowById(&windows, swpa->windowId);
                    if(wnd) moveWindow(&dirtyRect, wnd, swpa->x, swpa->y);
                    rpcIPCReturn(msg.Source, msg.ID, NULL, 0);
                }
                else if(!strcmp(req, "wmSetWindowTitle"))
                {
                    wmSetWindowTitleArgs *swta = (decltype(swta))(args);
                    Window *wnd = getWindowById(&windows, swta->windowId);
                    if(wnd)
                    {
                        wnd->SetTitle(swta->title);
                        dirtyRect = rcAdd(dirtyRect, wnd->GetDecoratedRect());
                    }
                    rpcIPCReturn(msg.Source, msg.ID, NULL, 0);
                }
                else printf("[windowmanager] Unknown RPC request '%s' from process %d\n", req, msg.Source);
            }
            else if(msg.Number == MSG_RPC_FIND_SERVER && !strcmp("windowmanager", (const char *)msg.Data))
                rpcIPCFindServerRespond(msg.Source, msg.ID);
        } while(ipcGetMessage(&msg, 0) >= 0);

        if(!rcIsEmptyP(&dirtyRect))
            updateRect(fbPixMap, bbPixMap, &windows, &dirtyRect);
    }

    printf("[windowmanager] Closing window manager\n");
    ipcSendMessage(0, MSG_RELEASE_KEYBOARD, MSG_FLAG_NONE, NULL, 0);
    ipcSendMessage(0, MSG_RELEASE_MOUSE, MSG_FLAG_NONE, NULL, 0);

    if(mouseWnd)
    {
        windows.RemoveOne(mouseWnd);
        delete mouseWnd;
    }
    if(desktopWnd)
    {
        windows.RemoveOne(desktopWnd);
        delete desktopWnd;
    }
    if(bbPixMap) pmDelete(bbPixMap);
    if(fbPixMap) pmDelete(fbPixMap);
    if(defaultFont) fntDelete(defaultFont);
    if(titleFont) fntDelete(titleFont);

    return 0;
}

Window *getWindowById(Windows *windows, int id)
{
    for(Window *wnd : *windows)
    {
        if(wnd->GetID() == id)
            return wnd;
    }
    return nullptr;
}

void moveWindow(rcRectangle_t *dirtyRect, Window *window, int x, int y)
{
    *dirtyRect = rcAdd(*dirtyRect, window->GetDecoratedRect());
    window->SetPosition(x, y);
    *dirtyRect = rcAdd(*dirtyRect, window->GetDecoratedRect());
}

void updateRect(pmPixMap_t *fb, pmPixMap_t *bb, Windows *windows, rcRectangle_t *rect)
{
    for(Window *wnd : *windows)
        wnd->UpdateWindowGraphics(bb, rect);
    pmBlit(fb, bb, rect->X, rect->Y, rect->X, rect->Y, rect->Width, rect->Height);
}
