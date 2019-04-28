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
    pmColor_t TextColor;
    pmColor_t BackColor;

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
    fntFont_t *Font;
    int CenterHorizontal;
};

struct uiButton
{
    uiControl_t Control;
    fntFont_t *Font;
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
    control->TextColor = pmColorBlack;
    control->BackColor = wmGetColor(WM_COLOR_BACKGROUND);
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
        pmFillRectangle(control->Content, 0, 0, rect.Width, rect.Height, control->BackColor);
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
    struct uiLabel *label = (struct uiLabel *)sender;
    rcRectangle_t rect = pmGetRectangle(label->Control.Content);
    float height = fntGetPixelHeight(label->Font);
    pmFillRectangle(sender->Content, 0, 0, rect.Width, rect.Height, sender->BackColor);
    if(label->CenterHorizontal)
    {
        float width = fntMeasureString(label->Font, label->Control.Text);
        fntDrawString(label->Font, label->Control.Content, (rect.Width - width) / 2, (rect.Height - height) / 2, label->Control.Text, label->Control.TextColor);
    }
    else fntDrawString(label->Font, label->Control.Content, 2, (rect.Height - height) / 2, label->Control.Text, label->Control.TextColor);
    pmInvalidateRect(label->Control.Content, rect);
}

uiLabel_t *uiLabelCreate(uiControl_t *parent, int x, int y, int width, int height, const char *text, fntFont_t *font, uiEventHandler onCreate)
{
    uiLabel_t *control = (uiLabel_t *)uiControlCreate(parent, sizeof(uiLabel_t), NULL, x, y, width, height, text, onCreate);
    if(!control) return NULL;
    control->Font = font ? font : wmGetFont(WM_FONT_DEFAULT);
    if(!control->Font)
    {
        uiLabelDelete(control);
        return NULL;
    }
    control->Control.OnPaint = labelPaint;
    return control;
}

void uiLabelSetHorizontalCentering(uiLabel_t *control, int value)
{
    if(!control) return;
    control->CenterHorizontal = value;
    if(control->Control.OnPaint)
        control->Control.OnPaint(&control->Control);
}

void uiLabelDelete(uiLabel_t *control)
{
    uiControlDelete((uiControl_t *)control);
}

static void buttonPaint(uiControl_t *sender)
{
    uiButton_t *button = (uiButton_t *)sender;
    rcRectangle_t rect = pmGetRectangle(button->Control.Content);
    float width = fntMeasureString(button->Font, button->Control.Text);
    float height = fntGetPixelHeight(button->Font);
    fntDrawString(button->Font, button->Control.Content, (rect.Width - width) / 2, (rect.Height - height) / 2, button->Control.Text, button->Control.TextColor);
    pmDrawFrame(button->Control.Content, 0, 0, rect.Width, rect.Height, 0);
    pmInvalidateRect(button->Control.Content, rect);
}

uiButton_t *uiButtonCreate(uiControl_t *parent, int x, int y, int width, int height, const char *text, fntFont_t *font, uiEventHandler onCreate)
{
    uiButton_t *control = (uiButton_t *)uiControlCreate(parent, sizeof(uiButton_t), NULL, x, y, width, height, text, onCreate);
    if(!control) return NULL;
    control->Font = font ? font : wmGetFont(WM_FONT_DEFAULT);
    if(!control->Font)
    {
        uiButtonDelete(control);
        return NULL;
    }
    control->Control.OnPaint = buttonPaint;
    return control;
}

void uiButtonDelete(uiButton_t *control)
{
    uiControlDelete((uiControl_t *)control);
}

static void lineEditPaint(uiControl_t *sender)
{
    uiLineEdit_t *edit = (uiLineEdit_t *)sender;
    rcRectangle_t rect = pmGetRectangle(edit->Control.Content);
    pmFillRectangle(edit->Control.Content, 0, 0, rect.Width, rect.Height, edit->Control.BackColor);
    float height = fntGetPixelHeight(edit->Font);
    fntDrawString(edit->Font, edit->Control.Content, 2, (rect.Height - height) / 2, edit->Control.Text, edit->Control.TextColor);
    pmDrawFrame(edit->Control.Content, 0, 0, rect.Width, rect.Height, 1);
    pmDrawFrame(edit->Control.Content, 1, 1, rect.Width - 2, rect.Height - 2, 1);
    pmInvalidateRect(edit->Control.Content, rect);
}

uiLineEdit_t *uiLineEditCreate(uiControl_t *parent, int x, int y, int width, int height, const char *text, fntFont_t *font, uiEventHandler onCreate)
{
    uiLineEdit_t *control = (uiLineEdit_t *)uiControlCreate(parent, sizeof(uiLineEdit_t), NULL, x, y, width, height, text, onCreate);
    if(!control) return NULL;
    control->Font = font ? font : wmGetFont(WM_FONT_DEFAULT);
    if(!control->Font)
    {
        uiLineEditDelete(control);
        return NULL;
    }
    control->Control.BackColor = pmColorWhite;
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
