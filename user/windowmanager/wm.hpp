#pragma once

#include <woot/input.h>
#include <woot/pixmap.h>
#include <woot/rpc.h>
#include <woot/ui.h>
#include <vector>

#include "window.hpp"

typedef struct inpMouseEvent inpMouseEvent_t;
typedef struct ipcMessage ipcMessage_t;
typedef struct uiToolbar uiToolbar_t;
typedef struct wmEvent wmEvent_t;

using namespace std;

class WindowManager
{
    typedef vector<Window *> Windows;

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

    struct wmRedrawRectArgs
    {
        int windowId;
        rcRectangle_t rect;
    };

    struct wmGetMousePosResp
    {
        int X, Y;
    };

    static void taskButtonActivate(uiControl_t *control);

    Windows *windows = nullptr;
    Window *desktopWnd = nullptr;
    rcRectangle_t deskRect;
    Window *mouseWnd = nullptr;
    pmPixMap_t *fbPixMap = nullptr;
    pmPixMap_t *bbPixMap = nullptr;
    Window *activeWindow = nullptr;
    Window *topWindow = nullptr;
    Window *dragWindow = nullptr;
    Window *taskWnd = nullptr;
    uiControl_t *taskRoot = nullptr;
    uiToolbar_t *taskBar = nullptr;
    rcRectangle_t taskRect;

    int caretTimer = 0;
    int caretVisible = 0;
    int modifiers = 0;
    int cursorHotX = 0, cursorHotY = 0;
    int dragDeltaX = 0, dragDeltaY = 0;
    int mouseX = 0, mouseY = 0;
    int oldMouseWndId = 0;
    int defX = 32, defY = 32;

    Window *getWindowById(int id);
    void moveWindow(rcRectangle_t *dirtyRect, Window *window, int x, int y);
    int getWindowIdx(Window *wnd);
    void bringToFront(Window *wnd);
    void sendToBack(Window *wnd);
    Window *findFrontWindow();
    Window *findBackWindow();
    Window *findByCoords(int x, int y);
    void handleMouseEnterLeave();
    void hideWindow(Window *wnd);
    void showWindow(Window *wnd);
    void setActiveWindow(Window *window);
    void wmEventMouse(rcRectangle_t *rect, int wndId, int mouseX, int mouseY, wmEvent_t *event, inpMouseEvent_t *mouseEv);
    void caretTick(int visible);
    void windowsRemove(Window *wnd);

public:
    WindowManager(pmPixMap_t *fbPixMap, pmPixMap_t *bbPixMap);
    int ProcessMessage(ipcMessage_t *msg, rcRectangle_t *dirtyRect);
    void UpdateRect(rcRectangle_t *rect);
    void GetMousePos(int *x, int *y);
    static void MinimizeWindow(Window *window);
    static void RestoreWindow(Window *window);    
    ~WindowManager();
};
