#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <woot/font.h>
#include <woot/pixmap.h>
#include <woot/rectangle.h>
#include <woot/ui.h>
#include <woot/wm.h>

struct uiControl
{
    uiControl_t *Next;
    uiControl_t *Parent;
    uiControl_t *Children;

    int X, Y;
    pmPixMap_t *Content;
    void *Context;
    char *Text;
    fntFont_t *Font;
    pmColor_t TextColor;
    pmColor_t BackColor;
    pmColor_t BorderColor;
    int TextHAlign;
    int TextVAlign;
    int BorderStyle;

    uiEventHandler OnCreate;
    uiEventHandler OnDelete;
    uiEventHandler OnPaint;

    uiWMEventHandler OnKeyPress;
    uiWMEventHandler OnKeyRelease;

    uiWMEventHandler PreMouseMove;
    uiWMEventHandler OnMouseMove;
    uiWMEventHandler PostMouseMove;

    uiWMEventHandler PreMousePress;
    uiWMEventHandler OnMousePress;
    uiWMEventHandler PostMousePress;

    uiWMEventHandler PreMouseRelease;
    uiWMEventHandler OnMouseRelease;
    uiWMEventHandler PostMouseRelease;
};

struct uiLabel
{
    uiControl_t Control;
};

struct uiButton
{
    uiControl_t Control;
};

struct uiLineEdit
{
    uiControl_t Control;
    fntFont_t *Font;
};

struct uiSlider
{
    uiControl_t Control;
    int Horizontal;
    int MinValue;
    int MaxValue;
    int Value;
};

static int mapValue(int imin, int imax, int omin, int omax, int val)
{
    return (float)(val - imin) / (imax - imin) * (omax - omin) + omin;
}

static void calculateTextPosition(uiControl_t *control, rcRectangle_t *rect, int textW, int textH, int *x, int *y)
{
    int borderWidth = 0;
    int borderHeight = 0;
    switch(control->BorderStyle)
    {
    case UI_BORDER_NONE:
        borderWidth = borderHeight = 0;
        break;
    case UI_BORDER_SIMPLE:
    case UI_BORDER_RAISED:
    case UI_BORDER_SUNKEN:
        borderWidth = borderHeight = 1;
        break;
    }
    *x = borderWidth;
    switch(control->TextHAlign)
    {
    case UI_HALIGN_LEFT:
        *x = borderWidth;
        break;
    case UI_HALIGN_CENTER:
        *x = (rect->Width - textW) / 2;
        break;
    case UI_HALIGN_RIGHT:
        *x = rect->Width - textW - borderWidth;
        break;
    }
    *y = borderHeight;
    switch(control->TextVAlign)
    {
    case UI_VALIGN_TOP:
        *y = borderHeight;
        break;
    case UI_VALIGN_MIDDLE:
        *y = (rect->Height - textH) / 2;
        break;
    case UI_VALIGN_BOTTOM:
        *y = rect->Height - textH - borderHeight;
        break;
    }
}

static void drawDefaultBorder(uiControl_t *control)
{
    rcRectangle_t rect = pmGetRectangle(control->Content);
    switch(control->BorderStyle)
    {
    case UI_BORDER_SIMPLE:
        pmRectangle(control->Content, 0, 0, rect.Width, rect.Height, control->BorderColor);
        break;
    case UI_BORDER_RAISED:
        pmDrawFrame(control->Content, 0, 0, rect.Width, rect.Height, 0);
        break;
    case UI_BORDER_SUNKEN:
        pmDrawFrame(control->Content, 0, 0, rect.Width, rect.Height, 1);
        break;
    }
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
    control->X = x;
    control->Y = y;
    control->Content = pmSubPixMap(parent ? parent->Content : parentPixMap, x, y, width, height);
    if(!control->Content)
    {
        uiControlDelete(control);
        return NULL;
    }
    control->Text = strdup(text ? text : "");
    control->Font = wmGetFont(WM_FONT_DEFAULT);
    control->TextColor = wmGetColor(WM_COLOR_TEXT);
    control->BackColor = wmGetColor(WM_COLOR_BACKGROUND);
    control->BorderColor = pmColorGetLuma(control->BackColor) > 128 ? pmColorBlack : pmColorWhite;
    control->TextHAlign = UI_HALIGN_CENTER;
    control->TextVAlign = UI_VALIGN_MIDDLE;
    control->BorderStyle = UI_BORDER_NONE;
    control->OnCreate = onCreate;
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
    if(control->Content)
        pmDelete(control->Content);
    if(control->Text)
        free(control->Text);
}

void uiControlRedraw(uiControl_t *control)
{
    if(control->BackColor.A != 0)
    {
        rcRectangle_t rect = pmGetRectangle(control->Content);
        if(control->BackColor.A == 255) pmFillRectangle(control->Content, 0, 0, rect.Width, rect.Height, control->BackColor);
        else pmAlphaRectangle(control->Content, 0, 0, rect.Width, rect.Height, control->BackColor);
    }
    if(control->OnPaint)
        control->OnPaint(control);
    for(uiControl_t *ctrl = control->Children; ctrl; ctrl = ctrl->Next)
        uiControlRedraw(ctrl);
}

pmPixMap_t *uiControlGetPixMap(uiControl_t *control)
{
    if(!control) return NULL;
    return control->Content;
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

char *uiControlGetText(uiControl_t *control)
{
    if(!control) return NULL;
    return control->Text;
}

void uiControlSetText(uiControl_t *control, const char *text)
{
    if(!control) return;
    if(control->Text) free(control->Text);
    control->Text = strdup(text);
}

int uiControlProcessEvent(uiControl_t *control, wmEvent_t event)
{
    if(!control) return -EINVAL;
    if(event.Type == WM_EVT_MOUSE)
    {
        for(uiControl_t *ctrl = control->Children; ctrl; ctrl = ctrl->Next)
        {
            rcRectangle_t rect = pmGetRectangle(ctrl->Content);
            if(rcContainsPointP(&rect, event.Mouse.Coords[0], event.Mouse.Coords[1]))
            {
                event.Mouse.Coords[0] -= rect.X;
                event.Mouse.Coords[1] -= rect.Y;
                int res = uiControlProcessEvent(ctrl, event);
                if(res) return res;
            }
        }

        if((event.Mouse.Delta[0] || event.Mouse.Delta[1]))
        {
            if(control->PreMouseMove) control->PreMouseMove(control, &event);
            if(control->OnMouseMove) control->OnMouseMove(control, &event);
            if(control->PostMouseMove) control->PostMouseMove(control, &event);
        }

        if(event.Mouse.ButtonsPressed)
        {
            if(control->PreMousePress) control->PreMousePress(control, &event);
            if(control->OnMousePress) control->OnMousePress(control, &event);
            if(control->PostMousePress) control->PostMousePress(control, &event);
        }

        if(control->OnMouseRelease && event.Mouse.ButtonsReleased)
        {
            if(control->PreMouseRelease) control->PreMouseRelease(control, &event);
            if(control->OnMouseRelease) control->OnMouseRelease(control, &event);
            if(control->PostMouseRelease) control->PostMouseRelease(control, &event);
        }
    }
    return 0;
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

static void labelPaint(struct uiControl *sender)
{
    rcRectangle_t rect = pmGetRectangle(sender->Content);
    int width = fntMeasureString(sender->Font, sender->Text);
    int height = fntGetPixelHeight(sender->Font);
    int x = 0, y = 0;
    calculateTextPosition(sender, &rect, width, height, &x, &y);
    fntDrawString(sender->Font, sender->Content, x, y, sender->Text, sender->TextColor);
    drawDefaultBorder(sender);
    pmInvalidateRect(sender->Content, rect);
}

uiLabel_t *uiLabelCreate(uiControl_t *parent, int x, int y, int width, int height, const char *text, uiEventHandler onCreate)
{
    uiLabel_t *control = (uiLabel_t *)uiControlCreate(parent, sizeof(uiLabel_t), NULL, x, y, width, height, text, onCreate);
    if(!control) return NULL;
    control->Control.OnPaint = labelPaint;
    return control;
}

void uiLabelDelete(uiLabel_t *control)
{
    uiControlDelete((uiControl_t *)control);
}

static void buttonPaint(uiControl_t *sender)
{
    rcRectangle_t rect = pmGetRectangle(sender->Content);
    int width = fntMeasureString(sender->Font, sender->Text);
    int height = fntGetPixelHeight(sender->Font);
    int x = 0, y = 0;
    calculateTextPosition(sender, &rect, width, height, &x, &y);
    fntDrawString(sender->Font, sender->Content, x, y, sender->Text, sender->TextColor);
    drawDefaultBorder(sender);
    pmInvalidateRect(sender->Content, rect);
}

uiButton_t *uiButtonCreate(uiControl_t *parent, int x, int y, int width, int height, const char *text, uiEventHandler onCreate)
{
    uiButton_t *control = (uiButton_t *)uiControlCreate(parent, sizeof(uiButton_t), NULL, x, y, width, height, text, onCreate);
    if(!control) return NULL;
    control->Control.BorderStyle = UI_BORDER_RAISED;
    control->Control.OnPaint = buttonPaint;
    return control;
}

void uiButtonDelete(uiButton_t *control)
{
    uiControlDelete((uiControl_t *)control);
}

static void lineEditPaint(uiControl_t *sender)
{
    rcRectangle_t rect = pmGetRectangle(sender->Content);
    int width = fntMeasureString(sender->Font, sender->Text);
    int height = fntGetPixelHeight(sender->Font);
    int x = 0, y = 0;
    calculateTextPosition(sender, &rect, width, height, &x, &y);
    fntDrawString(sender->Font, sender->Content, x, y, sender->Text, sender->TextColor);
    drawDefaultBorder(sender);
    pmInvalidateRect(sender->Content, rect);
}

uiLineEdit_t *uiLineEditCreate(uiControl_t *parent, int x, int y, int width, int height, const char *text, uiEventHandler onCreate)
{
    uiLineEdit_t *control = (uiLineEdit_t *)uiControlCreate(parent, sizeof(uiLineEdit_t), NULL, x, y, width, height, text, onCreate);
    if(!control) return NULL;
    control->Control.TextHAlign = UI_HALIGN_LEFT;
    control->Control.BackColor = pmColorWhite;
    control->Control.BorderStyle = UI_BORDER_SUNKEN;
    control->Control.OnPaint = lineEditPaint;
    return control;
}

void uiLineEditDelete(uiLineEdit_t *control)
{
    uiControlDelete((uiControl_t *)control);
}

static void sliderPaint(uiControl_t *control)
{
    if(!control) return;
    uiSlider_t *slider = (uiSlider_t *)control;
    rcRectangle_t rect = pmGetRectangle(slider->Control.Content);
    if(slider->Horizontal)
    {
        int cy = rect.Height / 2;
        pmDrawFrame(slider->Control.Content, 4, cy - 1, rect.Width - 8, 2, 1);
        int x = mapValue(slider->MinValue, slider->MaxValue, 4, rect.Width - 4, slider->Value);
        pmFillRectangle(slider->Control.Content, x - 2, 5, 4, rect.Height - 10, pmColorGray);
        pmDrawFrame(slider->Control.Content, x - 3, 4, 6, rect.Height - 8, 0);
    }
    else
    {
        int cx = rect.Width / 2;
        pmDrawFrame(slider->Control.Content, cx - 1, 4, 2, rect.Height - 8, 1);
        int y = mapValue(slider->MaxValue, slider->MinValue, 4, rect.Height - 4, slider->Value);
        pmFillRectangle(slider->Control.Content, 5, y - 2, rect.Width - 10, 4, pmColorGray);
        pmDrawFrame(slider->Control.Content, 4, y - 3, rect.Width - 8, 6, 0);
    }
    pmInvalidateRect(slider->Control.Content, rect);
}

uiSlider_t *uiSliderCreate(uiControl_t *parent, int x, int y, int width, int height, int horizontal, int minVal, int maxVal, int val)
{
    struct uiSlider *control = (struct uiSlider *)uiControlCreate(parent, sizeof(struct uiSlider), NULL, x, y, width, height, NULL, NULL);
    if(!control) return NULL;
    control->Horizontal = horizontal;
    if(maxVal < minVal)
    {
        int t = minVal;
        minVal = maxVal;
        maxVal = t;
    }
    control->MinValue = minVal;
    control->MaxValue = maxVal;
    if(val < minVal) val = minVal;
    else if(val > maxVal) val = maxVal;
    control->Value = val;
    control->Control.OnPaint = sliderPaint;
    return control;
}

void uiSliderSetValue(uiSlider_t *control, int value)
{
    if(!control) return;
    if(value < control->MinValue) value = control->MinValue;
    else if(value > control->MaxValue) value = control->MaxValue;
    control->Value = value;
    if(control->Control.OnPaint)
        control->Control.OnPaint(&control->Control);
}

int uiSliderGetValue(uiSlider_t *control)
{
    if(!control) return 0;
    return control->Value;
}

void uiSliderSetMinValue(uiSlider_t *control, int value)
{
    if(!control) return;
    control->MinValue = value;
    if(value > control->MaxValue) control->MaxValue = value;
    uiSliderSetValue(control, control->Value);
}

int uiSliderGetMinValue(uiSlider_t *control)
{
    if(!control) return 0;
    return control->MinValue;
}

void uiSliderSetMaxValue(uiSlider_t *control, int value)
{
    if(!control) return;
    control->MaxValue = value;
    if(value < control->MinValue) control->MinValue = value;
    uiSliderSetValue(control, control->Value);
}

int uiSliderGetMaxValue(uiSlider_t *control)
{
    if(!control) return 0;
    return control->MaxValue;
}

void uiSliderDelete(uiSlider_t *control)
{
    uiControlDelete((uiControl_t *)control);
}
