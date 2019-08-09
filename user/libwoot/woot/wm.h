#pragma once

#include <woot/pixmap.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// wmCreateWindow special positions
#define WM_CW_USEDEFAULT    0x80000000
#define WM_CW_CENTER        0x80000001

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
#define WM_CWF_POPUP            0x00000200
#define WM_CWF_HIDDEN           0x00000400

// default window flags for wmCreateWindow
#define WM_CWF_DEFAULT          (WM_CWF_APPWINDOW | WM_CWF_BORDER | WM_CWF_TITLEBAR | WM_CWF_CLOSEBUTTON | WM_CWF_MINIMIZEBUTTON | WM_CWF_MAXIMIZEBUTTON | WM_CWF_SHOWICON)

// color ids
typedef enum
{
    WM_COLOR_BACKGROUND = 0,
    WM_COLOR_TEXT,
    WM_COLOR_TITLE_BAR,
    WM_COLOR_INACTIVE_TITLE_BAR,
    WM_COLOR_TITLE_TEXT,
    WM_COLOR_INACTIVE_TITLE_TEXT,
    WM_COLOR_FOCUS_HIGHLIGHT,
    WM_COLOR_DEFAULT_TEXT,
    WM_COLOR_LINK,
    WM_COLOR_ID_COUNT // ALWAYS keep this entry last
} wmColorId_t;

// font ids
typedef enum
{
    WM_FONT_DEFAULT = 0,
    WM_FONT_UI_SYMBOLS,
    WM_FONT_TITLE_BAR,
    WM_FONT_MONO,
    WM_FONT_ID_COUNT // ALWAYS keep this entry last
} wmFontId_t;

// icon ids
typedef enum
{
    WM_ICON_FILE,
    WM_ICON_DIRECTORY,
    WM_ICON_PROGRAM,
    WM_ICON_ID_COUNT // ALWAYS keep this entry last
} wmIconId_t;

// event types
typedef enum
{
    WM_EVT_INVALID = 0,
    WM_EVT_OTHER,
    WM_EVT_CLOSE,
    WM_EVT_KEYBOARD,
    WM_EVT_MOUSE,
    WM_EVT_MOUSE_ENTER,
    WM_EVT_MOUSE_LEAVE,
    WM_EVT_CARET_TICK
} wmEventType_t;

// keyboard event definitions
#define WM_EVT_KB_RELEASED  1

// mouse event definitions
#define WM_EVT_MOUSE_AXES   5

// default dpi setting
#define WM_DEFAULT_DPI      96

typedef struct fntFont fntFont_t;
typedef struct rcRectangle rcRectangle_t;
typedef struct uiControl uiControl_t;
typedef struct wmWindow wmWindow_t;

typedef struct wmEvent
{
    union
    {
        wmEventType_t Type;
        unsigned TypeInt; // to make sure sizeof(Type) == sizeof(int)
    };
    unsigned WindowId;
    unsigned Handled;
    union
    {
        struct
        {
            unsigned Data[13];
        } Other;
        struct
        {
            unsigned Key;
            unsigned Flags;
            unsigned Character;
        } Keyboard;
        struct
        {
            unsigned Coords[WM_EVT_MOUSE_AXES];
            unsigned Delta[WM_EVT_MOUSE_AXES];
            unsigned ButtonsPressed;
            unsigned ButtonsReleased;
            unsigned ButtonsHeld;
        } Mouse;
        struct
        {
            unsigned Visible;
        } CaretTick;
    };
} wmEvent_t;

typedef void (*wmEventHandler)(wmWindow_t *sender, wmEvent_t *event);

#define WM_INITIALIZE_NONE  0
#define WM_INITIALIZE_WM    (1 << 0)

int wmInitialize(int flags);
int wmCleanup();
const char *wmGetServer();
int wmGetMousePos(int *x, int *y);
pmColor_t wmGetColor(wmColorId_t colorId);
fntFont_t *wmGetFont(wmFontId_t fontId);
pmPixMap_t *wmGetIcon(wmIconId_t iconId);
wmWindow_t *wmCreateWindow(int x, int y, unsigned w, unsigned h, unsigned flags);
int wmDeleteWindow(wmWindow_t *window);
int wmGetWindowId(wmWindow_t *window);
pmPixMap_t *wmGetPixMap(wmWindow_t *window);
void wmRedrawWindow(wmWindow_t *window);
void wmUpdateWindow(wmWindow_t *window);
void wmRedrawRect(wmWindow_t *window, rcRectangle_t *rect);
void wmSetWindowPos(wmWindow_t *window, int x, int y);
void wmSetWindowTitle(wmWindow_t *window, const char *title);
uiControl_t *wmGetRootControl(wmWindow_t *window);
int wmProcessEvent(wmWindow_t *window, wmEvent_t *event);
int wmShowWindow(wmWindow_t *window);
int wmHideWindow(wmWindow_t *window);
int wmActivateWindow(wmWindow_t *window);

int wmSetOnMouseEnter(wmWindow_t *window, wmEventHandler handler);
wmEventHandler wmGetOnMouseEnter(wmWindow_t *window);
int wmSetOnMouseLeave(wmWindow_t *window, wmEventHandler handler);
wmEventHandler wmGetOnMouseLeave(wmWindow_t *window);

#ifdef __cplusplus
}
#endif // __cplusplus
