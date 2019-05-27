#ifndef UI_H
#define UI_H

#include <woot/font.h>
#include <woot/pixmap.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// horizontal element alignment
#define UI_HALIGN_LEFT      0
#define UI_HALIGN_CENTER    1
#define UI_HALIGN_RIGHT     2

// vertical element alignment
#define UI_VALIGN_TOP       0
#define UI_VALIGN_MIDDLE    1
#define UI_VALIGN_BOTTOM    2

// border styles
#define UI_BORDER_NONE      0
#define UI_BORDER_SIMPLE    1
#define UI_BORDER_RAISED    2
#define UI_BORDER_SUNKEN    3

// control visibility
#define UI_HIDDEN           0
#define UI_VISIBLE          1

// icon position
#define UI_ICON_BEHIND      0
#define UI_ICON_OVER        1
#define UI_ICON_BELOW       2
#define UI_ICON_LEFT        3
#define UI_ICON_RIGHT       4

typedef struct wmEvent wmEvent_t;
typedef struct wmWindow wmWindow_t;

typedef struct uiControl uiControl_t;

typedef void (*uiEventHandler)(uiControl_t *sender);
typedef void (*uiWMEventHandler)(uiControl_t *sender, wmEvent_t *event);
typedef void (*uiGotFocusHandler)(uiControl_t *sender);
typedef void (*uiFocusLostHandler)(uiControl_t *sender);
typedef void (*uiActivateHandler)(uiControl_t *sender);

struct uiControl
{
    uiControl_t *Next;
    uiControl_t *Parent;
    uiControl_t *Children;
    wmWindow_t *Window;

    //int X, Y;
    rcRectangle_t Rectangle;
    pmPixMap_t *PixMap;
    void *Context;
    int Visibility;
    int CanHaveFocus;
    int HasFocus;
    char *Text;
    pmPixMap_t *Icon;
    fntFont_t *Font;
    pmColor_t TextColor;
    pmColor_t BackColor;
    pmColor_t BorderColor;
    int TextHAlign;
    int TextVAlign;
    int BorderStyle;
    int MarginSize;
    int IconPosition;
    int TextIconSeparation;

    uiEventHandler OnCreate;
    uiEventHandler OnDelete;
    uiEventHandler OnPaint;

    uiWMEventHandler PreKeyPress;
    uiWMEventHandler OnKeyPress;
    uiWMEventHandler PostKeyPress;

    uiWMEventHandler PreKeyRelease;
    uiWMEventHandler OnKeyRelease;
    uiWMEventHandler PostKeyRelease;

    uiWMEventHandler PreMouseMove;
    uiWMEventHandler OnMouseMove;
    uiWMEventHandler PostMouseMove;

    uiWMEventHandler PreMousePress;
    uiWMEventHandler OnMousePress;
    uiWMEventHandler PostMousePress;

    uiWMEventHandler PreMouseRelease;
    uiWMEventHandler OnMouseRelease;
    uiWMEventHandler PostMouseRelease;

    uiGotFocusHandler OnGotFocus;
    uiFocusLostHandler OnFocusLost;
    uiActivateHandler OnActivate;
};

uiControl_t *uiControlCreate(uiControl_t *parent, size_t structSize, pmPixMap_t *parentPixMap, int x, int y, int width, int height, const char *text, uiEventHandler onCreate);
void uiControlDelete(uiControl_t *control);
int uiControlGetPosition(uiControl_t *control, int *x, int *y, int global);
rcRectangle_t uiControlGetSize(uiControl_t *control);
rcRectangle_t uiControlGetRect(uiControl_t *control);
uiControl_t *uiControlGetRoot(uiControl_t *control);
uiControl_t *uiControlFindFocus(uiControl_t *control);
void uiControlSetFocus(uiControl_t *control);
void uiControlClearFocus(uiControl_t *control);
void uiControlRedraw(uiControl_t *control, int updateWindow);
void uiControlSetWindow(uiControl_t *control, wmWindow_t *window);
wmWindow_t *uiControlGetWindow(uiControl_t *control);
pmPixMap_t *uiControlGetPixMap(uiControl_t *control);
void *uiControlGetContext(uiControl_t *control);
void uiControlSetContext(uiControl_t *control, void *context);
void uiControlSetVisibility(uiControl_t *control, int visibility);
int uiControlGetVisibility(uiControl_t *control);
int uiControlHasFocus(uiControl_t *control);
char *uiControlGetText(uiControl_t *control);
void uiControlSetText(uiControl_t *control, const char *text);
void uiControlSetIcon(uiControl_t *control, pmPixMap_t *icon);
void uiControlSetFont(uiControl_t *control, fntFont_t *font);
int uiControlProcessEvent(uiControl_t *control, wmEvent_t *event);
void uiControlSetTextColor(uiControl_t *control, pmColor_t color);
void uiControlSetBackColor(uiControl_t *control, pmColor_t color);
void uiControlSetBorderColor(uiControl_t *control, pmColor_t color);
void uiControlSetTextHAlign(uiControl_t *control, int align);
void uiControlSetTextVAlign(uiControl_t *control, int align);
void uiControlSetBorderStyle(uiControl_t *control, int style);
void uiControlSetMarginSize(uiControl_t *control, int size);
void uiControlSetIconPosition(uiControl_t *control, int position);
void uiControlSetTextIconSeparation(uiControl_t *control, int separation);
void uiControlSetOnPaint(uiControl_t *control, uiEventHandler handler);
void uiControlSetOnMousePress(uiControl_t *control, uiWMEventHandler handler);
void uiControlSetOnMouseRelease(uiControl_t *control, uiWMEventHandler handler);
void uiControlSetOnMouseMove(uiControl_t *control, uiWMEventHandler handler);
void uiControlSetOnGotFocus(uiControl_t *control, uiGotFocusHandler handler);
void uiControlSetOnFocusLost(uiControl_t *control, uiFocusLostHandler handler);
void uiControlSetOnActivate(uiControl_t *control, uiActivateHandler handler);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // UI_H
