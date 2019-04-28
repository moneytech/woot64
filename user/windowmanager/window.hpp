#pragma once

typedef struct fntFont fntFont_t;
typedef struct pmPixMap pmPixMap_t;
typedef struct pmPixelFormat pmPixelFormat_t;
typedef struct rcRectangle rcRectangle_t;

class Window
{
    static int ids;

    int id;
    rcRectangle_t rect;
    unsigned flags;
    char *shMemName;
    void *pixels;
    int pixelsShMem;
    pmPixMap_t *pixMap;
    char *title;
public:
    static int TitleBarHeight;
    static fntFont_t *TitleFont;

    static Window *GetByID(int id);

    Window(int x, int y, unsigned w, unsigned h, unsigned flags, pmPixelFormat_t *format);
    int GetID() const;
    pmPixelFormat_t GetPixelFormat() const;
    const char *GetShMemName() const;
    rcRectangle_t GetRect() const;
    rcRectangle_t GetDecoratedRect() const;
    pmPixMap_t *GetPixMap() const;
    unsigned GetFlags() const;
    void SetPosition(int x, int y);
    void SetTitle(const char *title);
    const char *GetTitle() const;
    void UpdateWindowGraphics(pmPixMap_t *dst, rcRectangle_t *dstDirtyRect);
    ~Window();
};
