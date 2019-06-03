#pragma once

#include <woot/font.h>
#include <woot/pixmap.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// logic
typedef enum uiBool
{
    UI_FALSE = 0,
    UI_TRUE
} uiBool_t;

// orientation
typedef enum uiOrientation
{
    UI_HORIZONTAL = 0,
    UI_VERTICAL
} uiOrientation_t;

// horizontal element alignment
typedef enum uiHAlignment
{
    UI_HALIGN_LEFT = 0,
    UI_HALIGN_CENTER,
    UI_HALIGN_RIGHT
} uiHAlignment_t;

// vertical element alignment
typedef enum uiVAlignment
{
    UI_VALIGN_TOP = 0,
    UI_VALIGN_MIDDLE,
    UI_VALIGN_BOTTOM
} uiVAlignment_t;

// border styles
typedef enum uiBorderStyle
{
    UI_BORDER_NONE = 0,
    UI_BORDER_SIMPLE,
    UI_BORDER_RAISED,
    UI_BORDER_SUNKEN
} uiBorderStyle_t;

// control visibility
typedef enum uiVisibility
{
    UI_HIDDEN = 0,
    UI_VISIBLE
} uiVisibility_t;

// icon position
typedef enum uiRelPosition
{
    UI_BEHIND = 0,
    UI_OVER,
    UI_BELOW,
    UI_LEFT,
    UI_RIGHT
} uiRelPosition_t;

typedef struct wmEvent wmEvent_t;
typedef struct wmWindow wmWindow_t;

typedef struct uiControl uiControl_t;

typedef void (*uiEventHandler)(uiControl_t *sender);
typedef void (*uiWMEventHandler)(uiControl_t *sender, wmEvent_t *event);
typedef void (*uiGotFocusHandler)(uiControl_t *sender);
typedef void (*uiFocusLostHandler)(uiControl_t *sender);
typedef void (*uiActivateHandler)(uiControl_t *sender);
typedef void (*uiTextChangedHandler)(uiControl_t *sender);
typedef void (*uiRecalcRectsHandler)(uiControl_t *sender);

struct uiControl
{
    uiControl_t *Next;
    uiControl_t *Parent;
    uiControl_t *Children;
    wmWindow_t *Window;

    rcRectangle_t Rectangle;
    pmPixMap_t *PixMap;
    uiControl_t *LockedControl;
    void *Context;
    uiVisibility_t Visibility;
    uiBool_t CanHaveFocus;
    uiBool_t HasFocus;
    char *Text;
    pmPixMap_t *Icon;
    fntFont_t *Font;
    pmColor_t TextColor;
    pmColor_t BackColor;
    pmColor_t BorderColor;
    uiHAlignment_t TextHAlign;
    uiVAlignment_t TextVAlign;
    uiBorderStyle_t BorderStyle;
    int MarginSize;
    uiRelPosition_t IconPosition;
    int TextIconSeparation;

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
    uiTextChangedHandler OnTextChanged;
    uiRecalcRectsHandler OnRecalcRects;
};

uiControl_t *uiControlCreate(uiControl_t *parent, size_t structSize, pmPixMap_t *parentPixMap, int x, int y, int width, int height, const char *text);
void uiControlDelete(uiControl_t *control);
uiControl_t *uiControlGetParent(uiControl_t *control);
int uiControlGetPosition(uiControl_t *control, int *x, int *y, int global);
rcRectangle_t uiControlGetSize(uiControl_t *control);
rcRectangle_t uiControlGetRect(uiControl_t *control);
void uiControlSetRect(uiControl_t *control, rcRectangle_t rect);
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
void uiControlSetVisibility(uiControl_t *control, uiVisibility_t visibility);
uiVisibility_t uiControlGetVisibility(uiControl_t *control);
void uiControlSetCanHaveFocus(uiControl_t *control, uiBool_t value);
uiBool_t uiControlGetCanHaveFocus(uiControl_t *control);
uiBool_t uiControlHasFocus(uiControl_t *control);
char *uiControlGetText(uiControl_t *control);
void uiControlSetText(uiControl_t *control, const char *text);
void uiControlSetIcon(uiControl_t *control, pmPixMap_t *icon);
void uiControlSetFont(uiControl_t *control, fntFont_t *font);
int uiControlProcessEvent(uiControl_t *control, wmEvent_t *event);
int uiControlRecalcRects(uiControl_t *control);

// TODO: add getters
void uiControlSetTextColor(uiControl_t *control, pmColor_t color);
void uiControlSetBackColor(uiControl_t *control, pmColor_t color);
void uiControlSetBorderColor(uiControl_t *control, pmColor_t color);
void uiControlSetTextHAlign(uiControl_t *control, uiHAlignment_t align);
void uiControlSetTextVAlign(uiControl_t *control, uiVAlignment_t align);
void uiControlSetBorderStyle(uiControl_t *control, uiBorderStyle_t style);
void uiControlSetMarginSize(uiControl_t *control, int size);
void uiControlSetIconPosition(uiControl_t *control, uiRelPosition_t position);
void uiControlSetTextIconSeparation(uiControl_t *control, int separation);
void uiControlSetOnPaint(uiControl_t *control, uiEventHandler handler);
void uiControlSetOnMousePress(uiControl_t *control, uiWMEventHandler handler);
void uiControlSetOnMouseRelease(uiControl_t *control, uiWMEventHandler handler);
void uiControlSetOnMouseMove(uiControl_t *control, uiWMEventHandler handler);
void uiControlSetOnGotFocus(uiControl_t *control, uiGotFocusHandler handler);
void uiControlSetOnFocusLost(uiControl_t *control, uiFocusLostHandler handler);
void uiControlSetOnActivate(uiControl_t *control, uiActivateHandler handler);
void uiControlSetOnTextChanged(uiControl_t *control, uiTextChangedHandler handler);

#ifdef __cplusplus
}
#endif // __cplusplus
