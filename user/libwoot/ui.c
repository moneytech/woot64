#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <woot/font.h>
#include <woot/pixmap.h>
#include <woot/rectangle.h>
#include <woot/ui.h>
#include <woot/wm.h>

#undef max
#define max(x, y) ((x) > (y) ? (x) : (y))

static int mapValue(int imin, int imax, int omin, int omax, int val)
{
    return (float)(val - imin) / (imax - imin) * (omax - omin) + omin;
}

// calculate face (text and icon) size
static void calculateFaceSize(uiControl_t *control, int *w, int *h)
{
    // initialize return values
    if(w) *w = 0;
    if(h) *h = 0;

    if(!control->Icon)
    {   // no icon
        if(!control->Font || !control->Text || !control->Text[0])
            return; // no icon nor text
        if(w) *w = fntMeasureString(control->Font, control->Text);
        if(h) *h = fntGetPixelHeight(control->Font);
        return;
    }

    if(!control->Font || !control->Text || !control->Text[0])
    {   // no text/icon only
        if(w) *w = control->Icon->Contents.Width;
        if(h) *h = control->Icon->Contents.Height;
        return;
    }

    // we have both icon and text
    int textWidth = fntMeasureString(control->Font, control->Text);
    int textHeight = fntGetPixelHeight(control->Font);
    int iconWidth = control->Icon->Contents.Width;
    int iconHeight = control->Icon->Contents.Height;

    int width = 0;
    int height = 0;

    switch(control->IconPosition)
    {
    default:
    case UI_ICON_BEHIND:
        width = max(textWidth, iconWidth);
        height = max(textHeight, iconHeight);
        break;
    case UI_ICON_OVER:
    case UI_ICON_BELOW:
        width = max(textWidth, iconWidth);
        height = control->TextIconSeparation + textHeight + iconHeight;
        break;
    case UI_ICON_LEFT:
    case UI_ICON_RIGHT:
        width = control->TextIconSeparation + textWidth + iconWidth;
        height = max(textHeight, iconHeight);
        break;
    }

    if(w) *w = width;
    if(h) *h = height;
}

// calculate where to draw controls face
static void calculateFaceRect(uiControl_t *control, rcRectangle_t *rect, rcRectangle_t *faceRect)
{
    if(!faceRect)
        return;

    calculateFaceSize(control, &faceRect->Width, &faceRect->Height);

    int borderSize = 0;
    switch(control->BorderStyle)
    {
    case UI_BORDER_NONE:
        borderSize = 0;
        break;
    case UI_BORDER_SIMPLE:
    case UI_BORDER_RAISED:
    case UI_BORDER_SUNKEN:
        borderSize = 1;
        break;
    }
    borderSize += control->MarginSize;
    faceRect->X = borderSize;
    switch(control->TextHAlign)
    {
    case UI_HALIGN_LEFT:
        faceRect->X = borderSize;
        break;
    case UI_HALIGN_CENTER:
        faceRect->X = (rect->Width - faceRect->Width) / 2;
        break;
    case UI_HALIGN_RIGHT:
        faceRect->X = rect->Width - faceRect->Width - borderSize;
        break;
    }
    faceRect->Y = borderSize;
    switch(control->TextVAlign)
    {
    case UI_VALIGN_TOP:
        faceRect->Y = borderSize;
        break;
    case UI_VALIGN_MIDDLE:
        faceRect->Y = (rect->Height - faceRect->Height) / 2;
        break;
    case UI_VALIGN_BOTTOM:
        faceRect->Y = rect->Height - faceRect->Height - borderSize;
        break;
    }
}

static void drawDefaultFace(uiControl_t *control)
{
    rcRectangle_t rect = pmGetRectangle(control->PixMap);
    rcRectangle_t faceRect;
    calculateFaceRect(control, &rect, &faceRect);
    if(!control->Icon)
    {   // text only
        fntDrawString(control->Font, control->PixMap, faceRect.X, faceRect.Y, control->Text, control->TextColor);
        return;
    }

    if(!control->Font || !control->Text || !control->Text[0])
    {   // icon only
        pmAlphaBlit(control->PixMap, control->Icon, 0, 0, faceRect.X, faceRect.Y, -1, -1);
        return;
    }

    // text with icon
    int cx = faceRect.X + faceRect.Width / 2;
    int cy = faceRect.Y + faceRect.Height / 2;
    int textWidth = fntMeasureString(control->Font, control->Text);
    int textHeight = fntGetPixelHeight(control->Font);
    switch(control->IconPosition)
    {
    default:
    case UI_ICON_BEHIND:
        pmAlphaBlit(control->PixMap, control->Icon, 0, 0, cx - control->Icon->Contents.Width / 2, cy - control->Icon->Contents.Height / 2, -1, -1);
        fntDrawString(control->Font, control->PixMap, cx - textWidth / 2, cy - textHeight / 2, control->Text, control->TextColor);
        break;
    case UI_ICON_OVER:
        pmAlphaBlit(control->PixMap, control->Icon, 0, 0, cx - control->Icon->Contents.Width / 2, faceRect.Y, -1, -1);
        fntDrawString(control->Font, control->PixMap, cx - textWidth / 2, faceRect.Y + faceRect.Height - textHeight, control->Text, control->TextColor);
        break;
    case UI_ICON_BELOW:
        pmAlphaBlit(control->PixMap, control->Icon, 0, 0, cx - control->Icon->Contents.Width / 2, faceRect.Y + faceRect.Height - control->Icon->Contents.Height, -1, -1);
        fntDrawString(control->Font, control->PixMap, cx - textWidth / 2, faceRect.Y, control->Text, control->TextColor);
        break;
    case UI_ICON_LEFT:
        pmAlphaBlit(control->PixMap, control->Icon, 0, 0, faceRect.X, cy - control->Icon->Contents.Height / 2, -1, -1);
        fntDrawString(control->Font, control->PixMap, faceRect.X + faceRect.Width - textWidth, cy - textHeight / 2, control->Text, control->TextColor);
        break;
    case UI_ICON_RIGHT:
        pmAlphaBlit(control->PixMap, control->Icon, 0, 0, faceRect.X + faceRect.Width - control->Icon->Contents.Width, cy - control->Icon->Contents.Height / 2, -1, -1);
        fntDrawString(control->Font, control->PixMap, faceRect.X, cy - textHeight / 2, control->Text, control->TextColor);
        break;
    }
    if(control->HasFocus)
        pmRectanglePattern(control->PixMap, faceRect.X - 1, faceRect.Y - 1, faceRect.Width + 2, faceRect.Height + 2, 0x55555555, wmGetColor(WM_COLOR_FOCUS_HIGHLIGHT));
}

static void drawDefaultBorder(uiControl_t *control)
{
    rcRectangle_t rect = pmGetRectangle(control->PixMap);
    switch(control->BorderStyle)
    {
    case UI_BORDER_SIMPLE:
        pmRectangle(control->PixMap, 0, 0, rect.Width, rect.Height, control->BorderColor);
        break;
    case UI_BORDER_RAISED:
        pmDrawFrame(control->PixMap, 0, 0, rect.Width, rect.Height, 0, control->BackColor);
        break;
    case UI_BORDER_SUNKEN:
        pmDrawFrame(control->PixMap, 0, 0, rect.Width, rect.Height, 1, control->BackColor);
        break;
    }
}

// default OnPaint handler
static void defaultOnPaint(uiControl_t *sender)
{
    drawDefaultFace(sender);
    drawDefaultBorder(sender);
    //pmInvalidateWhole(sender->PixMap);
}

uiControl_t *uiControlCreate(uiControl_t *parent, size_t structSize, pmPixMap_t *parentPixMap, int x, int y, int width, int height, const char *text, uiEventHandler onCreate)
{
    uiControl_t *control = (uiControl_t *)calloc(1, structSize ? structSize : sizeof(uiControl_t));
    if(!control) return NULL;
    control->Parent = parent;
    if(control->Parent)
    {   // bind new control with the rest of the ui
        uiControl_t *ctrl;
        for(ctrl = control->Parent->Children; ctrl && ctrl->Next; ctrl = ctrl->Next);
        if(!ctrl) control->Parent->Children = control;
        else ctrl->Next = control;
    }
    control->Window = parent ? parent->Window : NULL;
    control->X = x;
    control->Y = y;
    control->PixMap = pmSubPixMap(parent ? parent->PixMap : parentPixMap, x, y, width, height);
    if(!control->PixMap)
    {
        uiControlDelete(control);
        return NULL;
    }
    control->Visibility = parent ? parent->Visibility : UI_VISIBLE;
    control->Text = strdup(text ? text : "");
    control->Icon = NULL;
    control->Font = parent ? parent->Font : wmGetFont(WM_FONT_DEFAULT);
    control->TextColor = parent ? parent->TextColor : wmGetColor(WM_COLOR_TEXT);
    control->BackColor = parent ? parent->BackColor : wmGetColor(WM_COLOR_BACKGROUND);
    control->BorderColor = parent ? parent->BorderColor : (pmColorGetLuma(control->BackColor) > 128 ? pmColorBlack : pmColorWhite);
    control->TextHAlign = UI_HALIGN_CENTER;
    control->TextVAlign = UI_VALIGN_MIDDLE;
    control->BorderStyle = UI_BORDER_NONE;
    control->MarginSize = 4;
    control->IconPosition = UI_ICON_OVER;
    control->OnCreate = onCreate;
    control->OnPaint = defaultOnPaint;
    if(control->OnCreate)
        control->OnCreate(control);
    return control;
}

void uiControlDelete(uiControl_t *control)
{
    if(!control) return;
    if(control->OnDelete)
        control->OnDelete(control);
    // delete child controls
    for(uiControl_t *ctrl = control->Children; ctrl; ctrl = ctrl->Next)
        uiControlDelete(ctrl);
    if(control->Parent)
    {   // unbind this control from the rest of the ui
        for(uiControl_t *ctrl = control->Parent->Children; ctrl; ctrl = ctrl->Next)
        {
            if(ctrl == control)
                control->Parent->Children = control->Next;
            else if(ctrl->Next == control)
            {
                ctrl->Next = control->Next;
                break;
            }
        }
    }
    if(control->PixMap)
        pmDelete(control->PixMap);
    if(control->Text)
        free(control->Text);
}

int uiControlGetPosition(uiControl_t *control, int *x, int *y, int global)
{
    if(!control) return -EINVAL;
    int rx = control->X;
    int ry = control->Y;
    uiControl_t *parent = control->Parent;
    while(global && parent)
    {
        rx += parent->X;
        ry += parent->Y;
        parent = parent->Parent;
    }
    if(x) *x = rx;
    if(y) *y = ry;
    return 0;
}

rcRectangle_t uiControlGetSize(uiControl_t *control)
{
    if(!control) return rcRectangleEmpty;
    rcRectangle_t rect = pmGetRectangle(control->PixMap);
    rect.X = rect.Y = 0;
    return rect;
}

uiControl_t *uiControlGetRoot(uiControl_t *control)
{
    if(!control) return NULL;
    uiControl_t *root = control;
    while(root->Parent)
        root = root->Parent;
    return root;
}

uiControl_t *uiControlFindFocus(uiControl_t *control)
{
    if(!control) return NULL;
    if(control->HasFocus) return control;
    for(uiControl_t *child = control->Children; child; child = child->Next)
    {
        uiControl_t *ctrl = uiControlFindFocus(child);
        if(ctrl) return ctrl;
    }
    return NULL;
}

void uiControlSetFocus(uiControl_t *control)
{
    if(!control) return;
    uiControl_t *root = uiControlGetRoot(control);
    uiControl_t *curFocus = uiControlFindFocus(root ? root : control);
    if(curFocus == control)
        return;
    uiControlClearFocus(root ? root : control);
    if(control->CanHaveFocus && !control->HasFocus)
    {
        control->HasFocus = 1;
        if(control->OnGotFocus)
            control->OnGotFocus(control);
        uiControlRedraw(control, 1);
    }
}

void uiControlClearFocus(uiControl_t *control)
{
    if(!control) return;
    for(uiControl_t *child = control->Children; child; child = child->Next)
        uiControlClearFocus(child);
    if(control->HasFocus)
    {
        control->HasFocus = 0;
        if(control->OnFocusLost)
            control->OnFocusLost(control);
        uiControlRedraw(control, 1);
    }
}

void uiControlRedraw(uiControl_t *control, int updateWindow)
{
    if(control->Visibility == UI_HIDDEN)
        return;
    if(control->BackColor.A != 0)
    {
        rcRectangle_t rect = pmGetRectangle(control->PixMap);
        if(control->BackColor.A == 255) pmFillRectangle(control->PixMap, 0, 0, rect.Width, rect.Height, control->BackColor);
        else pmAlphaRectangle(control->PixMap, 0, 0, rect.Width, rect.Height, control->BackColor);
    }
    if(control->OnPaint)
        control->OnPaint(control);
    for(uiControl_t *ctrl = control->Children; ctrl; ctrl = ctrl->Next)
        uiControlRedraw(ctrl, 0);
    pmPixMap_t *pm = uiControlGetPixMap(control);
    if(pm) pmInvalidateWhole(pm);
    if(updateWindow && control->Window)
        wmUpdateWindow(control->Window);
}

void uiControlSetWindow(uiControl_t *control, wmWindow_t *window)
{
    if(!control) return;
    control->Window = window;
}

wmWindow_t *uiControlGetWindow(uiControl_t *control)
{
    if(!control) return NULL;
    return control->Window;
}

pmPixMap_t *uiControlGetPixMap(uiControl_t *control)
{
    if(!control) return NULL;
    return control->PixMap;
}

void *uiControlGetContext(uiControl_t *control)
{
    if(!control) return NULL;
    return control->Context;
}

void uiControlSetContext(uiControl_t *control, void *context)
{
    if(!control) return;
    control->Context = context;
}

void uiControlSetVisibility(uiControl_t *control, int visibility)
{
    if(!control) return;
    control->Visibility = visibility;
}

int uiControlGetVisibility(uiControl_t *control)
{
    if(!control) return -EINVAL;
    return control->Visibility;
}

int uiControlHasFocus(uiControl_t *control)
{
    if(!control) return 0;
    return control->HasFocus ? 1 : 0;
}

char *uiControlGetText(uiControl_t *control)
{
    if(!control) return NULL;
    return control->Text;
}

void uiControlSetText(uiControl_t *control, const char *text)
{
    if(!control) return;
    if(control->Text != text)
    {
        if(control->Text) free(control->Text);
        control->Text = text ? strdup(text) : NULL;
    }
    uiControlRedraw(control, 1);
}

void uiControlSetIcon(uiControl_t *control, pmPixMap_t *icon)
{
    if(!control) return;
    control->Icon = icon;
}

void uiControlSetFont(uiControl_t *control, fntFont_t *font)
{
    if(!control) return;
    control->Font = font;
}

int uiControlProcessEvent(uiControl_t *control, wmEvent_t *event)
{
    if(!control) return -EINVAL;
    if(event->Type == WM_EVT_KEYBOARD)
    {
        uiControl_t *focus = uiControlFindFocus(control);
        if(focus)
        {
            if(event->Keyboard.Flags & WM_EVT_KB_RELEASED)
            {
                if(focus->PreKeyRelease) focus->PreKeyRelease(focus, event);
                if(focus->OnKeyRelease) focus->OnKeyRelease(focus, event);
                if(focus->PostKeyRelease) focus->PostKeyRelease(focus, event);
            }
            else
            {
                if(focus->PreKeyPress) focus->PreKeyPress(focus, event);
                if(focus->OnKeyPress) focus->OnKeyPress(focus, event);
                if(focus->PostKeyPress) focus->PostKeyPress(focus, event);
            }
        }
        return 1;
    }
    else if(event->Type == WM_EVT_MOUSE)
    {
        for(uiControl_t *ctrl = control->Children; ctrl; ctrl = ctrl->Next)
        {
            rcRectangle_t rect = pmGetRectangle(ctrl->PixMap);
            if(rcContainsPointP(&rect, event->Mouse.Coords[0], event->Mouse.Coords[1]))
            {
                int origX = event->Mouse.Coords[0];
                int origY = event->Mouse.Coords[1];
                event->Mouse.Coords[0] -= rect.X;
                event->Mouse.Coords[1] -= rect.Y;
                int res = uiControlProcessEvent(ctrl, event);
                event->Mouse.Coords[0] = origX;
                event->Mouse.Coords[1] = origY;
                if(res) return res;
            }
        }

        int res = 0;
        if((event->Mouse.Delta[0] || event->Mouse.Delta[1]))
        {
            if(control->PreMouseMove) control->PreMouseMove(control, event);
            if(control->OnMouseMove) control->OnMouseMove(control, event);
            if(control->PostMouseMove) control->PostMouseMove(control, event);
            res = 1;
        }

        if(event->Mouse.ButtonsPressed)
        {
            if(event->Mouse.ButtonsPressed & 1)
                uiControlSetFocus(control);

            if(control->PreMousePress) control->PreMousePress(control, event);
            if(control->OnMousePress) control->OnMousePress(control, event);
            if(control->PostMousePress) control->PostMousePress(control, event);
            res = 1;
        }

        if(event->Mouse.ButtonsReleased)
        {
            if(control->PreMouseRelease) control->PreMouseRelease(control, event);
            if(control->OnMouseRelease) control->OnMouseRelease(control, event);
            if(control->PostMouseRelease) control->PostMouseRelease(control, event);
            res = 1;
        }
        return res;
    }
    return 1;
}

void uiControlSetTextColor(uiControl_t *control, pmColor_t color)
{
    if(!control) return;
    control->TextColor = color;
}

void uiControlSetBackColor(uiControl_t *control, pmColor_t color)
{
    if(!control) return;
    control->BackColor = color;
}

void uiControlSetBorderColor(uiControl_t *control, pmColor_t color)
{
    if(!control) return;
    control->BorderColor = color;
}

void uiControlSetTextHAlign(uiControl_t *control, int align)
{
    if(!control) return;
    control->TextHAlign = align;
}

void uiControlSetTextVAlign(uiControl_t *control, int align)
{
    if(!control) return;
    control->TextVAlign = align;
}

void uiControlSetBorderStyle(uiControl_t *control, int style)
{
    if(!control) return;
    control->BorderStyle = style;
}

void uiControlSetMarginSize(uiControl_t *control, int size)
{
    if(!control) return;
    control->MarginSize = size;
}

void uiControlSetIconPosition(uiControl_t *control, int position)
{
    if(!control) return;
    control->IconPosition = position;
}

void uiControlSetTextIconSeparation(uiControl_t *control, int separation)
{
    if(!control) return;
    control->TextIconSeparation = separation;
}

void uiControlSetOnPaint(uiControl_t *control, uiEventHandler handler)
{
    if(!control) return;
    control->OnPaint = handler;
}

void uiControlSetOnMousePress(uiControl_t *control, uiWMEventHandler handler)
{
    if(!control) return;
    control->OnMousePress = handler;
}

void uiControlSetOnMouseRelease(uiControl_t *control, uiWMEventHandler handler)
{
    if(!control) return;
    control->OnMouseRelease = handler;
}

void uiControlSetOnMouseMove(uiControl_t *control, uiWMEventHandler handler)
{
    if(!control) return;
    control->OnMouseMove = handler;
}

void uiControlSetOnGotFocus(uiControl_t *control, uiGotFocusHandler handler)
{
    if(!control) return;
    control->OnGotFocus = handler;
}

void uiControlSetOnFocusLost(uiControl_t *control, uiFocusLostHandler handler)
{
    if(!control) return;
    control->OnFocusLost = handler;
}

void uiControlSetOnActivate(uiControl_t *control, uiActivateHandler handler)
{
    if(!control) return;
    control->OnActivate = handler;
}
