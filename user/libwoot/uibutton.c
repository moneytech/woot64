#include <errno.h>
#include <stdlib.h>
#include <woot/uibutton.h>
#include <woot/wm.h>

#undef max
#define max(x, y) ((x) > (y) ? (x) : (y))

struct uiButton
{
    uiControl_t Control;
    uiBool_t Pressed;
    uiBool_t Selected;
};

static void calculateFaceSize(uiButton_t *button, int *w, int *h)
{
    // initialize return values
    if(w) *w = 0;
    if(h) *h = 0;

    if(!button->Control.Icon)
    {   // no icon
        if(!button->Control.Font || !button->Control.Text || !button->Control.Text[0])
            return; // no icon nor text
        if(w) *w = fntMeasureString(button->Control.Font, button->Control.Text, -1);
        if(h) *h = fntGetPixelHeight(button->Control.Font);
        return;
    }

    if(!button->Control.Font || !button->Control.Text || !button->Control.Text[0])
    {   // no text/icon only
        if(w) *w = button->Control.Icon->Contents.Width;
        if(h) *h = button->Control.Icon->Contents.Height;
        return;
    }

    // we have both icon and text
    int textWidth = fntMeasureString(button->Control.Font, button->Control.Text, -1);
    int textHeight = fntGetPixelHeight(button->Control.Font);
    int iconWidth = button->Control.Icon->Contents.Width;
    int iconHeight = button->Control.Icon->Contents.Height;

    int width = 0;
    int height = 0;

    switch(button->Control.IconPosition)
    {
    default:
    case UI_BEHIND:
        width = max(textWidth, iconWidth);
        height = max(textHeight, iconHeight);
        break;
    case UI_OVER:
    case UI_BELOW:
        width = max(textWidth, iconWidth);
        height = button->Control.TextIconSeparation + textHeight + iconHeight;
        break;
    case UI_LEFT:
    case UI_RIGHT:
        width = button->Control.TextIconSeparation + textWidth + iconWidth;
        height = max(textHeight, iconHeight);
        break;
    }

    if(w) *w = width;
    if(h) *h = height;
}

static void calculateFaceRect(uiButton_t *button, rcRectangle_t *rect, rcRectangle_t *faceRect)
{
    if(!faceRect)
        return;

    calculateFaceSize(button, &faceRect->Width, &faceRect->Height);

    int borderSize = 0;
    switch(button->Control.BorderStyle)
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
    borderSize += button->Control.MarginSize;
    faceRect->X = borderSize;
    switch(button->Control.TextHAlign)
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
    switch(button->Control.TextVAlign)
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
    if(button->Pressed)
    {
        faceRect->X += 1;
        faceRect->Y += 1;
    }
}

static void buttonDrawFace(uiButton_t *button)
{
    rcRectangle_t faceRect;
    calculateFaceRect(button, &button->Control.Rectangle, &faceRect);
    if(!button->Control.Icon)
    {   // text only
        fntDrawString(button->Control.Font, button->Control.PixMap, faceRect.X, faceRect.Y, button->Control.Text, button->Control.TextColor);
        if(button->Control.HasFocus)
            pmRectanglePattern(button->Control.PixMap, faceRect.X - 1, faceRect.Y - 1, faceRect.Width + 2, faceRect.Height + 2, 0x55555555, wmGetColor(WM_COLOR_FOCUS_HIGHLIGHT));
        return;
    }

    if(!button->Control.Font || !button->Control.Text || !button->Control.Text[0])
    {   // icon only
        pmAlphaBlit(button->Control.PixMap, button->Control.Icon, 0, 0, faceRect.X, faceRect.Y, -1, -1);
        if(button->Control.HasFocus)
            pmRectanglePattern(button->Control.PixMap, faceRect.X - 1, faceRect.Y - 1, faceRect.Width + 2, faceRect.Height + 2, 0x55555555, wmGetColor(WM_COLOR_FOCUS_HIGHLIGHT));
        return;
    }

    // text with icon
    int cx = faceRect.X + faceRect.Width / 2;
    int cy = faceRect.Y + faceRect.Height / 2;
    int textWidth = fntMeasureString(button->Control.Font, button->Control.Text, -1);
    int textHeight = fntGetPixelHeight(button->Control.Font);
    switch(button->Control.IconPosition)
    {
    default:
    case UI_BEHIND:
        pmAlphaBlit(button->Control.PixMap, button->Control.Icon, 0, 0, cx - button->Control.Icon->Contents.Width / 2, cy - button->Control.Icon->Contents.Height / 2, -1, -1);
        fntDrawString(button->Control.Font, button->Control.PixMap, cx - textWidth / 2, cy - textHeight / 2, button->Control.Text, button->Control.TextColor);
        break;
    case UI_OVER:
        pmAlphaBlit(button->Control.PixMap, button->Control.Icon, 0, 0, cx - button->Control.Icon->Contents.Width / 2, faceRect.Y, -1, -1);
        fntDrawString(button->Control.Font, button->Control.PixMap, cx - textWidth / 2, faceRect.Y + faceRect.Height - textHeight, button->Control.Text, button->Control.TextColor);
        break;
    case UI_BELOW:
        pmAlphaBlit(button->Control.PixMap, button->Control.Icon, 0, 0, cx - button->Control.Icon->Contents.Width / 2, faceRect.Y + faceRect.Height - button->Control.Icon->Contents.Height, -1, -1);
        fntDrawString(button->Control.Font, button->Control.PixMap, cx - textWidth / 2, faceRect.Y, button->Control.Text, button->Control.TextColor);
        break;
    case UI_LEFT:
        pmAlphaBlit(button->Control.PixMap, button->Control.Icon, 0, 0, faceRect.X, cy - button->Control.Icon->Contents.Height / 2, -1, -1);
        fntDrawString(button->Control.Font, button->Control.PixMap, faceRect.X + faceRect.Width - textWidth, cy - textHeight / 2, button->Control.Text, button->Control.TextColor);
        break;
    case UI_RIGHT:
        pmAlphaBlit(button->Control.PixMap, button->Control.Icon, 0, 0, faceRect.X + faceRect.Width - button->Control.Icon->Contents.Width, cy - button->Control.Icon->Contents.Height / 2, -1, -1);
        fntDrawString(button->Control.Font, button->Control.PixMap, faceRect.X, cy - textHeight / 2, button->Control.Text, button->Control.TextColor);
        break;
    }
    if(button->Control.HasFocus)
        pmRectanglePattern(button->Control.PixMap, faceRect.X - 1, faceRect.Y - 1, faceRect.Width + 2, faceRect.Height + 2, 0x55555555, wmGetColor(WM_COLOR_FOCUS_HIGHLIGHT));
}

static void buttonDrawBorder(uiButton_t *button)
{
    rcRectangle_t rect = button->Control.Rectangle;
    rect.X = rect.Y = 0;
    int pressed = (button->Pressed != 0) != (button->Selected != 0);
    switch(button->Control.BorderStyle)
    {
    default:
        break;
    case UI_BORDER_SIMPLE:
        pmRectangleRect(button->Control.PixMap, &rect, pressed ? pmColorInvert(button->Control.BorderColor) : button->Control.BorderColor);
        break;
    case UI_BORDER_RAISED:
        pmDrawFrameRect(button->Control.PixMap, &rect, pressed != 0 ? 1 : 0, button->Control.BackColor);
        break;
    case UI_BORDER_SUNKEN:
        pmDrawFrameRect(button->Control.PixMap, &rect, pressed != 0 ? 0 : 1, button->Control.BackColor);
        break;
    }
}

static void buttonOnPaint(uiControl_t *sender)
{
    if(!sender) return;
    rcRectangle_t rect = pmGetRectangle(sender->PixMap);
    uiButton_t *button = (uiButton_t *)sender;

    // draw background
    pmColor_t backColor = button->Selected ? pmColorBrighten(sender->BackColor, 0.1f) : sender->BackColor;
    if(sender->BackColor.A == 255)
        pmFillRectangle(sender->PixMap, 0, 0, rect.Width, rect.Height, backColor);
    else pmAlphaRectangle(sender->PixMap, 0, 0, rect.Width, rect.Height, backColor);

    buttonDrawFace(button);
    buttonDrawBorder(button);
}

static void buttonPreMousePress(uiControl_t *control, wmEvent_t *event)
{
    if(!control || !event) return;
    if(!(event->Mouse.ButtonsPressed & 1))
        return;
    uiButton_t *button = (uiButton_t *)control;
    button->Pressed = 1;
    uiControlRedraw(control, 1);
}

static void buttonPreMouseMove(uiControl_t *control, wmEvent_t *event)
{
    if(!control || !event) return;
    uiButton_t *button = (uiButton_t *)control;
    int origPressed = button->Pressed;
    if(!(event->Mouse.ButtonsHeld & 1))
        button->Pressed = 0;
    if(button->Pressed == origPressed)
        return;
    uiControlRedraw(control, 1);
}

static void buttonPostMouseRelease(uiControl_t *control, wmEvent_t *event)
{
    if(!control || !event) return;
    if(!(event->Mouse.ButtonsReleased & 1))
        return;
    uiButton_t *button = (uiButton_t *)control;
    int pressed = button->Pressed;
    button->Pressed = 0;
    uiControlRedraw(control, 1);
    if(pressed)
    {
        if(control->OnActivate)
            control->OnActivate(control);
    }
}

uiButton_t *uiButtonCreate(uiControl_t *parent, int x, int y, int width, int height, const char *text)
{
    uiButton_t *control = (uiButton_t *)uiControlCreate(parent, sizeof(uiButton_t), NULL, x, y, width, height, text);
    if(!control) return NULL;
    control->Control.CanHaveFocus = 1;
    control->Control.BorderStyle = UI_BORDER_RAISED;
    control->Control.OnPaint = buttonOnPaint;
    control->Control.PreMousePress = buttonPreMousePress;
    control->Control.PreMouseMove = buttonPreMouseMove;
    control->Control.PostMouseRelease = buttonPostMouseRelease;
    return control;
}

void uiButtonDelete(uiButton_t *control)
{
    uiControlDelete((uiControl_t *)control);
}

void uiButtonSetSelected(uiButton_t *button, uiBool_t value)
{
    if(!button) return;
    button->Selected = value;
    uiControlRedraw(&button->Control, 1);
}

uiBool_t uiButtonGetSelected(uiButton_t *button)
{
    if(!button) return -EINVAL;
    return button->Selected;
}
