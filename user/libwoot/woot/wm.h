#pragma once

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// wmCreateWindow default position
#define WM_CW_USEDEFAULT 0x80000000

// wmCreateWindow flags
#define WM_CWF_NONE             0x00000000
#define WM_CWF_APPWINDOW        0x00000001
#define WM_CWF_BORDER           0x00000002
#define WM_CWF_TITLEBAR         0x00000004
#define WM_CWF_CLOSEBUTTON      0x00000008
#define WM_CWF_MINIMIZEBUTTON   0x00000010
#define WM_CWF_MAXIMIZEBUTTON   0x00000020
#define WM_CWF_HELPBUTTON       0x00000040
#define WM_CWF_SHOWICON         0x00000080
#define WM_CWF_USEALPHA         0x00000100

// default window flags for wmCreateWindow
#define WM_CWF_DEFAULT          (WM_CWF_APPWINDOW | WM_CWF_BORDER | WM_CWF_TITLEBAR | WM_CWF_CLOSEBUTTON | WM_CWF_MINIMIZEBUTTON | WM_CWF_MAXIMIZEBUTTON | WM_CWF_SHOWICON)

// window messages
#define WM_QUIT             1

// color ids
#define WM_COLOR_BACKGROUND 0
#define WM_COLOR_TEXT       1
#define WM_COLOR_ID_COUNT   2

// font ids
#define WM_FONT_DEFAULT     0
#define WM_FONT_ID_COUNT    1

// event types
#define WM_EVT_INVALID      0
#define WM_EVT_OTHER        1
#define WM_EVT_KEYBOARD     2
#define WM_EVT_MOUSE        3

// keyboard event definitions
#define WM_EVT_KB_RELEASED  1

// mouse event definitions
#define WM_EVT_MOUSE_AXES   5

// default dpi setting
#define WM_DEFAULT_DPI      96

typedef struct fntFont fntFont_t;
typedef union pmColor pmColor_t;
typedef struct pmPixMap pmPixMap_t;
typedef struct pmPixelFormat pmPixelFormat_t;
typedef struct uiControl uiControl_t;
typedef struct wmWindow wmWindow_t;

typedef struct wmEvent
{
    int Type;
    union
    {
        struct
        {
            int Data[15];
        } Other;
        struct
        {
            int Key;
            int Flags;
        } Keyboard;
        struct
        {
            int Coords[WM_EVT_MOUSE_AXES];
            int Delta[WM_EVT_MOUSE_AXES];
            int ButtonsPressed;
            int ButtonsReleased;
            int ButtonsHeld;
        } Mouse;
    };
} wmEvent_t;

int wmInitialize();
int wmCleanup();
const char *wmGetServer();
pmColor_t wmGetColor(int colorId);
fntFont_t *wmGetFont(int fontId);
wmWindow_t *wmCreateWindow(int x, int y, unsigned w, unsigned h, unsigned flags);
int wmDeleteWindow(wmWindow_t *window);
pmPixMap_t *wmGetPixMap(wmWindow_t *window);
void wmRedrawWindow(wmWindow_t *window);
void wmSetWindowPos(wmWindow_t *window, int x, int y);
void wmSetWindowTitle(wmWindow_t *window, const char *title);
uiControl_t *wmGetRootControl(wmWindow_t *window);

#ifdef __cplusplus
}
#endif // __cplusplus
