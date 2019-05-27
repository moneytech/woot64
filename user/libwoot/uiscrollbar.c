#include <stdlib.h>
#include <woot/uibutton.h>
#include <woot/uiscrollbar.h>
#include <woot/vkeys.h>
#include <woot/wm.h>

#undef max
#define max(x, y) ((x) > (y) ? (x) : (y))
#undef min
#define min(x, y) ((x) < (y) ? (x) : (y))

struct uiScrollbar
{
    uiControl_t Control;
    int Horizontal;
    int MinPosition;
    int MaxPosition;
    int Position;
    int Zoom;

    uiScrollbarChangePositionHandler OnChangePosition;

    int Dragging;
    uiButton_t *LeftUpButton;
    uiButton_t *RightDownButton;
};

static int mapValue(int imin, int imax, int omin, int omax, int val)
{
    return (float)(val - imin) / (imax - imin) * (omax - omin) + omin;
}

static void scrollbarUpdate(uiScrollbar_t *scrollbar, wmEvent_t *event)
{
    if(event && event->Type == WM_EVT_MOUSE)
    {
        rcRectangle_t rect = scrollbar->Control.Rectangle;
        rect.X = rect.Y = 0;

        int btnSize = min(rect.Width, rect.Height);
        int trackSize = (scrollbar->Horizontal ? rect.Width : rect.Height) - btnSize * 2;
        int thumbStart = mapValue(scrollbar->MinPosition, scrollbar->MaxPosition, 0, trackSize, scrollbar->Position);
        int thumbEnd = mapValue(scrollbar->MinPosition, scrollbar->MaxPosition, 0, trackSize, scrollbar->Position + scrollbar->Zoom);
        int thumbCenter = (thumbEnd - thumbStart) / 2;

        int mousePos = (scrollbar->Horizontal ? event->Mouse.Coords[0] : event->Mouse.Coords[1]) - (btnSize + thumbCenter);
        int position = mapValue(0, trackSize, scrollbar->MinPosition, scrollbar->MaxPosition, mousePos);

        // clamp position
        if(position < scrollbar->MinPosition)
            position = scrollbar->MinPosition;
        else if((position + scrollbar->Zoom) > scrollbar->MaxPosition)
            position = scrollbar->MaxPosition - scrollbar->Zoom;
        scrollbar->Position = position;
    }
    if(scrollbar->OnChangePosition)
        scrollbar->OnChangePosition(scrollbar);
    uiControlRedraw((uiControl_t *)scrollbar, 1);
}

static void scrollbarPaint(uiControl_t *control)
{
    if(!control) return;
    uiScrollbar_t *scroll = (uiScrollbar_t *)control;
    rcRectangle_t rect = scroll->Control.Rectangle;
    pmPixMap_t *pm = control->PixMap;
    int btnSize = min(rect.Width, rect.Height);
    rect.X = rect.Y = 0;

    int trackSize = (scroll->Horizontal ? rect.Width : rect.Height) - btnSize * 2;
    int thumbStart = mapValue(scroll->MinPosition, scroll->MaxPosition, 0, trackSize, scroll->Position);
    int thumbEnd = mapValue(scroll->MinPosition, scroll->MaxPosition, 0, trackSize, scroll->Position + scroll->Zoom);
    int thumbSize = thumbEnd - thumbStart;

    pmColor_t backColor = pmColorBrighten(control->BackColor, 0.1f);
    pmColor_t thumbColor = control->HasFocus ? pmColorBrighten(control->BackColor, -0.1f) : control->BackColor;
    if(scroll->Horizontal)
    {
        pmFillRectangle(pm, btnSize, 0, rect.Width - btnSize * 2, rect.Height, backColor);
        pmFillRectangle(pm, btnSize + thumbStart, 0, thumbSize, rect.Height, thumbColor);
        pmDrawFrame(pm, btnSize + thumbStart, 0, thumbSize, rect.Height, 0,  control->BackColor);
    }
    else
    {
        pmFillRectangle(pm, 0, btnSize, rect.Width, rect.Height - btnSize * 2, backColor);
        pmFillRectangle(pm, 0, btnSize + thumbStart, rect.Width, thumbSize, thumbColor);
        pmDrawFrame(pm, 0, btnSize + thumbStart, rect.Width, thumbSize, 0, control->BackColor);
    }
}

static void scrollbarPreMousePress(uiControl_t *control, wmEvent_t *event)
{
    if(!control || !event) return;
    if(!(event->Mouse.ButtonsPressed & 1))
        return;
    uiScrollbar_t *scrollbar = (uiScrollbar_t *)control;
    scrollbar->Dragging = 1;
    scrollbarUpdate(scrollbar, event);
}

static void scrollbarPreMouseMove(uiControl_t *control, wmEvent_t *event)
{
    if(!control || !event) return;
    uiScrollbar_t *scrollbar = (uiScrollbar_t *)control;
    if(!(event->Mouse.ButtonsHeld & 1))
        scrollbar->Dragging = 0;
    if(!scrollbar->Dragging) return;
    scrollbarUpdate(scrollbar, event);
}

static void scrollbarPostMouseRelease(uiControl_t *control, wmEvent_t *event)
{
    if(!control || !event) return;
    if(!(event->Mouse.ButtonsReleased & 1))
        return;
    uiScrollbar_t *scrollbar = (uiScrollbar_t *)control;
    scrollbar->Dragging = 0;
}

static void scrollbarPreKeyPress(uiControl_t *control, wmEvent_t *event)
{
    if(!control || !event) return;
    uiScrollbar_t *scrollbar = (uiScrollbar_t *)control;
    if(scrollbar->Horizontal)
    {
        if(event->Keyboard.Key == VK_LEFT)
            uiScrollbarSetPosition(scrollbar, uiScrollbarGetPosition(scrollbar) - scrollbar->Zoom / 2);
        else if(event->Keyboard.Key == VK_UP)
            uiScrollbarSetPosition(scrollbar, uiScrollbarGetPosition(scrollbar) + scrollbar->Zoom / 2);
    }
    else
    {
        if(event->Keyboard.Key == VK_UP)
            uiScrollbarSetPosition(scrollbar, uiScrollbarGetPosition(scrollbar) - scrollbar->Zoom / 2);
        else if(event->Keyboard.Key == VK_DOWN)
            uiScrollbarSetPosition(scrollbar, uiScrollbarGetPosition(scrollbar) + scrollbar->Zoom / 2);
    }
}

static void onButtonActivate(uiControl_t *control)
{
    if(!control) return;
    uiButton_t *button = (uiButton_t *)control;
    uiScrollbar_t *scrollbar = (uiScrollbar_t *)control->Context;
    if(button == scrollbar->LeftUpButton)
        uiScrollbarSetPosition(scrollbar, uiScrollbarGetPosition(scrollbar) - scrollbar->Zoom / 2);
    else uiScrollbarSetPosition(scrollbar, uiScrollbarGetPosition(scrollbar) + scrollbar->Zoom / 2);
}

uiScrollbar_t *uiScrollbarCreate(uiControl_t *parent, int x, int y, int width, int height, int horizontal, int minPos, int maxPos, int pos, int zoom)
{
    uiScrollbar_t *control = (uiScrollbar_t *)uiControlCreate(parent, sizeof(uiScrollbar_t), NULL, x, y, width, height, NULL, NULL);
    if(!control) return NULL;

    int btnSize = min(width, height);
    control->LeftUpButton = uiButtonCreate((uiControl_t *)control, 0, 0, btnSize, btnSize, horizontal ? "<" : "^", NULL);
    if(!control->LeftUpButton)
    {
        uiScrollbarDelete(control);
        return NULL;
    }
    control->RightDownButton = uiButtonCreate((uiControl_t *)control, horizontal ? width - btnSize : 0, horizontal ? 0 : height - btnSize, btnSize, btnSize, horizontal ? ">" : "v", NULL);
    if(!control->RightDownButton)
    {
        uiScrollbarDelete(control);
        return NULL;
    }

    fntFont_t *symFont = wmGetFont(WM_FONT_UI_SYMBOLS);
    if(symFont)
    {
        uiControlSetFont((uiControl_t *)control->LeftUpButton, symFont);
        uiControlSetFont((uiControl_t *)control->RightDownButton, symFont);
    }

    uiControlSetContext((uiControl_t *)control->LeftUpButton, control);
    uiControlSetContext((uiControl_t *)control->RightDownButton, control);
    uiControlSetOnActivate((uiControl_t *)control->LeftUpButton, onButtonActivate);
    uiControlSetOnActivate((uiControl_t *)control->RightDownButton, onButtonActivate);

    control->Control.CanHaveFocus = 1;
    control->Horizontal = horizontal;

    if(maxPos < minPos)
    {
        int t = minPos;
        minPos = maxPos;
        maxPos = t;
    }
    if(zoom > (maxPos - minPos))
        zoom = maxPos - minPos;

    control->MinPosition = minPos;
    control->MaxPosition = maxPos;
    if(pos < minPos) pos = minPos;
    else if((pos + zoom) > maxPos) pos = maxPos - zoom;
    control->Position = pos;
    control->Zoom = zoom;

    control->Control.OnPaint = scrollbarPaint;
    control->Control.PreKeyPress = scrollbarPreKeyPress;
    control->Control.PreMousePress = scrollbarPreMousePress;
    control->Control.PreMouseMove = scrollbarPreMouseMove;
    control->Control.PostMouseRelease = scrollbarPostMouseRelease;
    return control;
}

void uiScrollbarDelete(uiScrollbar_t *control)
{
    uiControlDelete((uiControl_t *)control);
}

void uiScrollbarSetPosition(uiScrollbar_t *control, int position)
{
    if(!control) return;
    uiScrollbar_t *scrollbar = (uiScrollbar_t *)control;

    // clamp position
    if(position < scrollbar->MinPosition)
        position = scrollbar->MinPosition;
    else if((position + scrollbar->Zoom) > scrollbar->MaxPosition)
        position = scrollbar->MaxPosition - scrollbar->Zoom;

    scrollbar->Position = position;
    scrollbarUpdate(control, NULL);
}

int uiScrollbarGetPosition(uiScrollbar_t *control)
{
    if(!control) return 0;
    return control->Position;
}

void uiScrollbarSetZoom(uiScrollbar_t *control, int zoom)
{
    if(!control) return;
    uiScrollbar_t *scrollbar = (uiScrollbar_t *)control;

    // clamp zoom value
    if(zoom < 1) zoom = 1;
    else if(zoom > (scrollbar->MaxPosition - scrollbar->MinPosition))
        zoom = scrollbar->MaxPosition - scrollbar->MinPosition;

    scrollbar->Zoom = zoom;
    scrollbarUpdate(control, NULL);
}

int uiScrollbarGetZoom(uiScrollbar_t *control)
{
    if(!control) return 0;
    return control->Zoom;
}

void uiScrollbarSetMinPosition(uiScrollbar_t *control, int position)
{
    if(!control) return;

    if(position > control->MaxPosition)
        position = control->MaxPosition;

    if(control->Position < position)
        control->Position = position;

    control->MinPosition = position;

    if(control->Zoom > (control->MaxPosition - control->MinPosition))
        control->Zoom = (control->MaxPosition - control->MinPosition);

    scrollbarUpdate(control, NULL);
}

int uiScrollbarGetMinPosition(uiScrollbar_t *control)
{
    if(!control) return 0;
    return control->MinPosition;
}

void uiScrollbarSetMaxPosition(uiScrollbar_t *control, int position)
{
    if(!control) return;

    if(position < control->MinPosition)
        position = control->MinPosition;

    if(control->Position > position)
        control->Position = position;

    control->MaxPosition = position;

    if(control->Zoom > (control->MaxPosition - control->MinPosition))
        control->Zoom = (control->MaxPosition - control->MinPosition);

    scrollbarUpdate(control, NULL);
}

int uiScrollbarGetMaxPosition(uiScrollbar_t *control)
{
    if(!control) return 0;
    return control->MaxPosition;
}
