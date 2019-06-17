#pragma once

#include <woot/rectangle.h>

typedef struct fntFont fntFont_t;
typedef struct pmPixelFormat pmPixelFormat_t;
typedef struct pmPixMap pmPixMap_t;
typedef struct rcRectangle rcRectangle_t;
typedef struct uiButton uiButton_t;
typedef struct uiControl uiControl_t;
typedef struct uiLabel uiLabel_t;

class WindowManager;

class Window
{
    static int ids;

    int id;
    int owner;
    rcRectangle_t rect;
    unsigned flags;
    char *shMemName;
    void *pixels;
    int pixelsShMem;
    pmPixMap_t *pixMap;
    char *title;
    bool active;
    pmPixMap_t *dstPixMap;
    pmPixMap_t *titleBarPixMap;
    uiControl_t *titleBar;
    uiLabel_t *titleBarText;
    uiButton_t *closeButton;
    uiButton_t *maxButton;
    uiButton_t *minButton;
    uiButton_t *progButton;

    static void titleBarButtonActivate(uiControl_t *sender);
public:
    static int TitleBarHeight;

    static Window *GetByID(int id);

    uiButton_t *TaskButton;
    WindowManager *WM;
    bool Visible;

    Window(WindowManager *wm, int owner, int x, int y, unsigned w, unsigned h, unsigned flags, pmPixMap_t *dstPixMap, pmPixelFormat *pfOverride);
    int GetId() const;
    int GetOwner() const;
    pmPixelFormat_t GetPixelFormat() const;
    const char *GetShMemName() const;
    rcRectangle_t GetRect() const;
    rcRectangle_t GetDecoratedRect() const;
    rcRectangle_t GetTitleRect() const;
    rcRectangle_t GetDragRect() const;
    pmPixMap_t *GetPixMap() const;
    unsigned GetFlags() const;
    void SetPosition(int x, int y);
    void SetTitle(const char *title);
    const char *GetTitle() const;
    void UpdateWindowGraphics(rcRectangle_t *dstDirtyRect);
    rcRectangle_t SetActive(bool active);
    bool GetActive() const;
    uiControl_t *GetTitleControl() const;
    ~Window();
};
