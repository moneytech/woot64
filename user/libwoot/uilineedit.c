#include <stdlib.h>
#include <string.h>
#include <woot/font.h>
#include <woot/uilineedit.h>
#include <woot/wm.h>
#include <woot/vkeys.h>

struct uiLineEdit
{
    uiControl_t Control;
    int CursorPosition;
    int EditOffset;
};

static char *stringInsert(char *str, int *pos, char chr)
{
    size_t len = strlen(str);
    if(*pos < 0 || *pos > len)
        *pos = len;
    if(chr == '\b')
    {
        if(len < 1 || *pos < 1)
            return str;
        memmove(str + *pos - 1, str + *pos, len - *pos + 1);
        --(*pos);
        return str;
    }
    str = realloc(str, len + 2);
    memmove(str + *pos + 1, str + *pos, len - *pos + 1);
    str[(*pos)++] = chr;
    return str;
}

static void lineEditDrawBorder(uiControl_t *control)
{
    rcRectangle_t rect = control->Rectangle;
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

static void calculateTextSize(uiControl_t *control, int *w, int *h)
{
    // initialize return values
    if(w) *w = 0;
    if(h) *h = 0;

    if(!control->Font)
        return; // no font

    if(w) *w = fntMeasureString(control->Font, control->Text, -1);
    if(h) *h = fntGetPixelHeight(control->Font);
}

static void calculateTextRect(uiControl_t *control, rcRectangle_t *rect, rcRectangle_t *faceRect)
{
    if(!faceRect)
        return;

    calculateTextSize(control, &faceRect->Width, &faceRect->Height);

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

static void drawText(uiControl_t *control)
{
    rcRectangle_t faceRect;
    calculateTextRect(control, &control->Rectangle, &faceRect);

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

    uiLineEdit_t *edit = (uiLineEdit_t *)control;
    int cursorOffs = fntMeasureString(control->Font, control->Text, edit->CursorPosition);

    int textOffs = edit->EditOffset;

    if(control->HasFocus)
    {
        int curPos = faceRect.X + cursorOffs + textOffs;
        if(curPos < borderSize)
            textOffs += borderSize - curPos;
        else if(curPos > (control->Rectangle.Width - borderSize))
            textOffs += (control->Rectangle.Width - borderSize) - curPos;
    }
    else textOffs = 0;

    fntDrawString(control->Font, control->PixMap, faceRect.X + textOffs, faceRect.Y, control->Text, control->TextColor);

    if(control->HasFocus)
        pmVLine(control->PixMap, faceRect.X + cursorOffs + textOffs, faceRect.Y, faceRect.Y + faceRect.Height, control->TextColor);

    edit->EditOffset = textOffs;
}

static void lineEditOnPaint(uiControl_t *sender)
{
    drawText(sender);
    lineEditDrawBorder(sender);
}

static void lineEditPreKeyPress(uiControl_t *control, wmEvent_t *event)
{
    if(!control || !event) return;
    uiLineEdit_t *edit = (uiLineEdit_t *)control;
    int chr = event->Keyboard.Character;
    if(event->Keyboard.Key == VK_LEFT)
    {
        if(edit->CursorPosition > 0)
            --edit->CursorPosition;
        uiControlRedraw(control, 1);
        return;
    }
    else if(event->Keyboard.Key == VK_RIGHT)
    {
        int len = strlen(control->Text);
        if(edit->CursorPosition >= 0 && edit->CursorPosition < len)
            ++edit->CursorPosition;
        uiControlRedraw(control, 1);
        return;
    }
    else if(event->Keyboard.Key == VK_HOME)
    {
        edit->CursorPosition = 0;
        uiControlRedraw(control, 1);
        return;
    }
    else if(event->Keyboard.Key == VK_END)
    {
        edit->CursorPosition = -1;
        uiControlRedraw(control, 1);
        return;
    }
    else if(event->Keyboard.Key == VK_RETURN)
        return;

    if(!chr) return;

    control->Text = stringInsert(control->Text, &edit->CursorPosition, chr);
    uiControlRedraw(control, 1);
}

uiLineEdit_t *uiLineEditCreate(uiControl_t *parent, int x, int y, int width, int height, const char *text, uiEventHandler onCreate)
{
    uiLineEdit_t *control = (uiLineEdit_t *)uiControlCreate(parent, sizeof(uiLineEdit_t), NULL, x, y, width, height, text, onCreate);
    if(!control) return NULL;
    control->Control.CanHaveFocus = 1;
    control->Control.TextHAlign = UI_HALIGN_LEFT;
    control->Control.BackColor = pmColorWhite;
    control->Control.BorderStyle = UI_BORDER_SUNKEN;
    control->Control.PreKeyPress = lineEditPreKeyPress;
    control->Control.OnPaint = lineEditOnPaint;

    control->CursorPosition = -1; // -1 means end of text
    control->EditOffset = 0;
    return control;
}

void uiLineEditDelete(uiLineEdit_t *control)
{
    uiControlDelete((uiControl_t *)control);
}
