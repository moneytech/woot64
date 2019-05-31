#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <woot/font.h>
#include <woot/ipc.h>
#include <woot/pixmap.h>
#include <woot/rectangle.h>
#include <woot/rpc.h>
#include <woot/ui.h>
#include <woot/wm.h>

#define DEFAULT_RPC_TIMEOUT 1000

static char wmServer[64] = { 0 };
static pmColor_t colors[WM_COLOR_ID_COUNT];
static fntFont_t *fonts[WM_FONT_ID_COUNT];
static const char *fontNames[WM_FONT_ID_COUNT] =
{
    "/data/common/fonts/default.ttf",
    "/data/common/fonts/uisymbols.ttf",
    "/data/common/fonts/title.ttf"
};
static const int fontSizes[WM_FONT_ID_COUNT] =
{
    11,
    12,
    12
};

struct wmWindow
{
    int id;
    int shMemHandle;
    pmPixMap_t *pixMap;
    uiControl_t *rootControl;
};

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

int wmInitialize(int flags)
{
    int res = flags & WM_INITIALIZE_WM ? 0 :
                rpcFindServer("windowmanager", wmServer, sizeof(wmServer), DEFAULT_RPC_TIMEOUT);
    if(res < 0) return res;
    memset(colors, 0, sizeof(colors));
    colors[WM_COLOR_BACKGROUND] = pmColorSeaGreen;
    colors[WM_COLOR_TEXT] = pmColorBlack;
    colors[WM_COLOR_TITLE_BAR] = pmColorSlateBlue;
    colors[WM_COLOR_INACTIVE_TITLE_BAR] = pmColorSeaGreen;
    colors[WM_COLOR_TITLE_TEXT] = pmColorWhite;
    colors[WM_COLOR_INACTIVE_TITLE_TEXT] = pmColorSilver;
    colors[WM_COLOR_FOCUS_HIGHLIGHT] = pmColorBlack;
    memset(fonts, 0, sizeof(fonts));
    return res;
}

int wmCleanup()
{
    if(fonts[WM_FONT_DEFAULT]) fntDelete(fonts[WM_FONT_DEFAULT]);
    memset(fonts, 0, sizeof(fonts));
    return 0;
}

const char *wmGetServer()
{
    return wmServer[0] ? wmServer : NULL;
}

pmColor_t wmGetColor(int colorId)
{
    if(colorId < 0 || colorId >= WM_COLOR_ID_COUNT)
        return pmColorBlack;
    return colors[colorId];
}

fntFont_t *wmGetFont(int fontId)
{
    if(fontId < 0 || fontId >= WM_FONT_ID_COUNT)
        return NULL;

    // load fonts on demand
    if(!fonts[fontId])
    {
        fntFont_t *font = fntLoad(fontNames[fontId]);
        if(!font) return NULL;
        fntSetPointSize(font, fontSizes[fontId], WM_DEFAULT_DPI);
        fonts[fontId] = font;
    }

    return fonts[fontId];
}

wmWindow_t *wmCreateWindow(int x, int y, unsigned w, unsigned h, unsigned flags)
{
    int args[5] = { x, y, w, h, flags };
    union wmCreateWindowResp response;
    int res = rpcCall(wmServer, "wmCreateWindow", args, sizeof(args), &response, sizeof(response), DEFAULT_RPC_TIMEOUT);
    //printf("id: %d %s\n", response.id, response.shMemName);
    if(res < 0 || response.id < 0) return NULL;
    wmWindow_t *wnd = (wmWindow_t *)calloc(1, sizeof(wmWindow_t));
    if(!wnd)
    {
        errno = ENOMEM;
        return NULL;
    }
    wnd->id = response.id;
    wnd->shMemHandle = ipcOpenSharedMem(response.shMemName);
    if(wnd->shMemHandle < 0)
    {
        free(wnd);
        return NULL;
    }
    void *pixels = ipcMapSharedMem(wnd->shMemHandle, NULL, IPC_SHMEM_WRITE);
    if(!pixels)
    {
        ipcCloseSharedMem(wnd->shMemHandle);
        free(wnd);
        return NULL;
    }
    unsigned pitch = pmFormatPixelsToBytesP(&response.pixelFormat, w);
    wnd->pixMap = pmFromMemory(w, h, pitch, &response.pixelFormat, pixels, 0);
    if(!wnd->pixMap)
    {
        ipcUnMapSharedMem(wnd->shMemHandle, pixels);
        ipcCloseSharedMem(wnd->shMemHandle);
        free(wnd);
        return NULL;
    }
    wnd->rootControl = uiControlCreate(NULL, 0, wnd->pixMap, 0, 0, w, h, NULL);
    if(!wnd->rootControl)
    {
        pmDelete(wnd->pixMap);
        ipcUnMapSharedMem(wnd->shMemHandle, pixels);
        ipcCloseSharedMem(wnd->shMemHandle);
        free(wnd);
        return NULL;
    }
    uiControlSetWindow(wnd->rootControl, wnd);
    return wnd;
}

int wmDeleteWindow(wmWindow_t *window)
{
    if(!window) return -EINVAL;
    int response = -ENOSYS;
    int res = rpcCall(wmServer, "wmDeleteWindow", &window->id, sizeof(int), &response, sizeof(response), DEFAULT_RPC_TIMEOUT);
    if(res < 0) return res;
    if(window->rootControl)
        uiControlDelete(window->rootControl);
    if(window->pixMap)
    {
        void *pixels = window->pixMap->Pixels;
        pmDelete(window->pixMap);
        ipcUnMapSharedMem(window->shMemHandle, pixels);
        ipcCloseSharedMem(window->shMemHandle);
    }
    free(window);
    return response;
}

int wmGetWindowId(wmWindow_t *window)
{
    return window->id;
}

pmPixMap_t *wmGetPixMap(wmWindow_t *window)
{
    return window->pixMap;
}

void wmRedrawWindow(wmWindow_t *window)
{
    rpcCall(wmServer, "wmRedrawWindow", &window->id, sizeof(int), NULL, 0, 0);//DEFAULT_RPC_TIMEOUT);
}

void wmUpdateWindow(wmWindow_t *window)
{
    if(!window) return;
    rcRectangle_t rect = pmGetAndClearDirtyRectangle(window->pixMap);
    if(rcIsEmptyP(&rect))
        return;
    wmRedrawRect(window, &rect);
}

void wmRedrawRect(wmWindow_t *window, rcRectangle_t *rect)
{
    struct wmRedrawRectArgs args = { window->id, *rect };
    rpcCall(wmServer, "wmRedrawRect", &args, sizeof(args), NULL, 0, 0);//DEFAULT_RPC_TIMEOUT);
}

void wmSetWindowPos(wmWindow_t *window, int x, int y)
{
    struct wmSetWindowPosArgs args = { window->id, x, y };
    rpcCall(wmServer, "wmSetWindowPos", &args, sizeof(args), NULL, 0, 0);//DEFAULT_RPC_TIMEOUT);
}

void wmSetWindowTitle(wmWindow_t *window, const char *title)
{
    union
    {
        char rpcArgs[MSG_RPC_RESP_PAYLOAD_SIZE];
        struct wmSetWindowTitleArgs args;
    } a;
    int titleLen = strlen(title);
    a.args.windowId = window->id;
    memcpy((void *)a.args.title, title, titleLen + 1);
    rpcCall(wmServer, "wmSetWindowTitle", &a.args, sizeof(a.args) + titleLen + 1, NULL, 0, 0);//DEFAULT_RPC_TIMEOUT);
}

uiControl_t *wmGetRootControl(wmWindow_t *window)
{
    if(!window)
    {
        errno = EINVAL;
        return NULL;
    }
    return window->rootControl;
}

int wmProcessEvent(wmWindow_t *window, wmEvent_t *event)
{
    if(window->id != event->WindowId)
        return -EINVAL;
    if(!window->rootControl)
        return -EINVAL;
    return uiControlProcessEvent(window->rootControl, event);
}
