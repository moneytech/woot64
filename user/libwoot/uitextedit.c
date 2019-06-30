#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <woot/uiscrollbar.h>
#include <woot/uitextedit.h>
#include <woot/vector.h>
#include <woot/vkeys.h>
#include <woot/wm.h>

#undef max
#define max(x, y) ((x) > (y) ? (x) : (y))
#undef min
#define min(x, y) ((x) < (y) ? (x) : (y))

struct uiTextEdit
{
    uiControl_t Control;
    vecVector_t *Lines;
    uiScrollbar_t *HScroll;
    uiScrollbar_t *VScroll;
    uiControl_t *TextContainer;
    int CursorColumn, CursorRow;
    int MaxLineWidth;
    uiBool_t Editable;
    uiBool_t Shift;
    uiBool_t Ctrl;

    uiBool_t CaretVisible;
    int CaretX, CaretY, CaretH;
};

static void invertVLine(pmPixMap_t *pixMap, int x, int y, int h)
{
    for(int ey = y + h; y < ey; ++y)
        pmSetPixel(pixMap, x, y, pmColorInvert(pmGetPixel(pixMap, x, y)));
}

static void onCaretPaint(uiControl_t *sender)
{
    uiTextEdit_t *edit = (uiTextEdit_t *)sender->Parent;
    if(edit->CaretVisible) return;
    invertVLine(sender->PixMap, edit->CaretX, edit->CaretY, edit->CaretH);
    pmInvalidate(sender->PixMap, edit->CaretX, edit->CaretY, 1, edit->CaretH);
    edit->CaretVisible = 1;
    if(sender->Window) wmUpdateWindow(sender->Window);
}

static void onCaretClear(uiControl_t *sender)
{
    uiTextEdit_t *edit = (uiTextEdit_t *)sender->Parent;
    if(!edit->CaretVisible) return;
    invertVLine(sender->PixMap, edit->CaretX, edit->CaretY, edit->CaretH);
    pmInvalidate(sender->PixMap, edit->CaretX, edit->CaretY, 1, edit->CaretH);
    edit->CaretVisible = 0;
    if(sender->Window) wmUpdateWindow(sender->Window);
}

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
    else if(chr == 127)
    {
        if(*pos < 0 || *pos >= len)
            return str;
        memmove(str + *pos, str + *pos + 1, len - *pos + 1);
        return str;
    }
    str = realloc(str, len + 2);
    memmove(str + *pos + 1, str + *pos, len - *pos + 1);
    str[(*pos)++] = chr;
    return str;
}

static void textEditUpdate(uiTextEdit_t *edit)
{
    unsigned lineCount = vecSize(edit->Lines);
    fntFont_t *font = edit->Control.Font;
    float lineHeight = fntGetPixelAscender(font);
    float lineDesc = fntGetPixelDescender(font);
    int viewHeight = edit->TextContainer->Rectangle.Height - (edit->TextContainer->BorderStyle == UI_BORDER_NONE ? 0 : 2);
    uiScrollbarSetMaxPosition(edit->VScroll, lineHeight * lineCount - lineDesc);
    uiScrollbarSetZoom(edit->VScroll, viewHeight);
    uiScrollbarSetStep(edit->VScroll, lineHeight);

    int viewWidth = edit->TextContainer->Rectangle.Width - (edit->TextContainer->BorderStyle == UI_BORDER_NONE ? 0 : 2);
    uiScrollbarSetMaxPosition(edit->HScroll, edit->MaxLineWidth);
    uiScrollbarSetZoom(edit->HScroll, viewWidth);
    uiScrollbarSetStep(edit->HScroll, fntMeasureCharacter(font, 'A'));

    int vPos = uiScrollbarGetPosition(edit->VScroll);
    int evPos = vPos + uiScrollbarGetZoom(edit->VScroll);
    unsigned startLine = vPos / lineHeight;
    unsigned endLine = evPos / lineHeight;
    int redrawScroll = 0;
    if(edit->CursorRow < startLine)
    {
        uiScrollbarSetPosition(edit->VScroll, lineHeight * edit->CursorRow);
        redrawScroll = 1;
    }
    else if(edit->CursorRow >= (endLine - 1))
    {
        uiScrollbarSetPosition(edit->VScroll, lineHeight * (edit->CursorRow + 1) - viewHeight - lineDesc);
        redrawScroll = 1;
    }

    char **linePtr = (char **)vecGet(edit->Lines, edit->CursorRow);
    if(linePtr)
    {
        char *line = *linePtr;
        size_t lineLen = strlen(line);
        edit->CursorColumn = min(lineLen, edit->CursorColumn);
        int hPos = uiScrollbarGetPosition(edit->HScroll);
        int curX = fntMeasureString(font, line, edit->CursorColumn);
        if(curX < hPos)
        {
            uiScrollbarSetPosition(edit->HScroll, curX);
            redrawScroll = 1;
        }
        else if(curX > (hPos + viewWidth))
        {
            uiScrollbarSetPosition(edit->HScroll, curX - viewWidth);
            redrawScroll = 1;
        }

        if(redrawScroll)
        {
            uiControlRedraw((uiControl_t *)edit->VScroll, 0);
            uiControlRedraw((uiControl_t *)edit->HScroll, 0);
        }
    }
}

static void recalcMaxWidth(uiTextEdit_t *edit)
{
    fntFont_t *font = edit->Control.Font;
    int lineCount = vecSize(edit->Lines);
    int maxWidth = 0;
    for(unsigned i = 0; i < lineCount; ++i)
    {
        char *line = *(char **)vecGet(edit->Lines, i);
        int curWidth = fntMeasureString(font, line, -1);
        if(curWidth > maxWidth)
            maxWidth = curWidth;
    }
    edit->MaxLineWidth = maxWidth;
}

static void textContainerDrawBorder(uiControl_t *control)
{
    rcRectangle_t rect = control->Rectangle;
    pmColor_t frameColor = control->Parent ? control->Parent->BackColor : wmGetColor(WM_COLOR_BACKGROUND);
    switch(control->BorderStyle)
    {
    default:
        break;
    case UI_BORDER_SIMPLE:
        pmRectangle(control->PixMap, 0, 0, rect.Width, rect.Height, control->BorderColor);
        break;
    case UI_BORDER_RAISED:
        pmDrawFrame(control->PixMap, 0, 0, rect.Width, rect.Height, 0, frameColor);
        break;
    case UI_BORDER_SUNKEN:
        pmDrawFrame(control->PixMap, 0, 0, rect.Width, rect.Height, 1, frameColor);
        break;
    }
}

static void textContainerOnPaint(uiControl_t *sender)
{
    rcRectangle_t rect = pmGetRectangle(sender->PixMap);
    if(sender->BackColor.A == 255)
        pmFillRectangle(sender->PixMap, 0, 0, rect.Width, rect.Height, sender->BackColor);
    else pmAlphaRectangle(sender->PixMap, 0, 0, rect.Width, rect.Height, sender->BackColor);

    uiTextEdit_t *edit = (uiTextEdit_t *)(sender->Parent);
    fntFont_t *font = edit->Control.Font;
    float lineHeight = fntGetPixelAscender(font);
    float descender = fntGetPixelDescender(font);
    pmPixMap_t *pm = sender->PixMap;
    pmColor_t textColor = edit->Control.TextColor;
    unsigned lineCount = vecSize(edit->Lines);
    int vPos = uiScrollbarGetPosition(edit->VScroll);
    int evPos = vPos + uiScrollbarGetZoom(edit->VScroll);
    unsigned startLine = vPos / lineHeight;
    unsigned endLine = (evPos + lineHeight - 1) / lineHeight;
    int xoffs = -uiScrollbarGetPosition(edit->HScroll);
    for(unsigned i = startLine; i < endLine && i < lineCount; ++i)
    {
        char *line = *(char **)vecGet(edit->Lines, i);
        if(!line) continue;

        int y = 1 + i * lineHeight - vPos;

        fntDrawString(font, pm, 2 + xoffs, y, line, textColor);

        if(i == edit->CursorRow)
        {
            int curX = fntMeasureString(font, line, edit->CursorColumn);
            edit->CaretX = 1 + xoffs + curX;
            edit->CaretY = y;
            edit->CaretH = lineHeight - descender;

            if(sender->HasFocus)
            {
                edit->CaretVisible = UI_FALSE;
                onCaretPaint(sender);
            }
        }
    }

    textContainerDrawBorder(sender);
}

static void scrollChangePosition(uiScrollbar_t *scroll)
{
    uiTextEdit_t *edit = (uiTextEdit_t *)uiControlGetParent((uiControl_t *)scroll);
    uiControlRedraw(edit->TextContainer, 1);
}

static void editOnGotFocus(uiControl_t *sender)
{   // redirect focus to the editor itself
    uiTextEdit_t *edit = (uiTextEdit_t *)sender;
    uiControlSetFocus(edit->TextContainer);
}

static void textPreKeyPress(uiControl_t *control, wmEvent_t *event)
{
    if(!control || !event) return;
    uiTextEdit_t *edit = (uiTextEdit_t *)control->Parent;
    int chr = event->Keyboard.Character;
    if(event->Keyboard.Key == VK_LSHIFT || event->Keyboard.Key == VK_RSHIFT)
        edit->Shift = UI_TRUE;
    else if(event->Keyboard.Key == VK_LCONTROL || event->Keyboard.Key == VK_RCONTROL)
        edit->Ctrl = UI_TRUE;
    else if(event->Keyboard.Key == VK_UP)
    {
        if(edit->CursorRow > 0)
            --edit->CursorRow;
        textEditUpdate(edit);
        uiControlRedraw(control, 1);
    }
    else if(event->Keyboard.Key == VK_DOWN)
    {
        int lineCount = vecSize(edit->Lines);
        if(edit->CursorRow < (lineCount - 1))
            ++edit->CursorRow;
        textEditUpdate(edit);
        uiControlRedraw(control, 1);
    }
    else if(event->Keyboard.Key == VK_LEFT)
    {
        if(edit->CursorColumn > 0)
            --edit->CursorColumn;
        else
        {
            if(edit->CursorRow > 0)
            {
                char **linePtr = (char **)vecGet(edit->Lines, edit->CursorRow - 1);
                if(linePtr)
                {
                    char *line = *linePtr;
                    edit->CursorColumn = strlen(line);
                        --edit->CursorRow;
                } else edit->CursorRow = 0;
            }
        }
        textEditUpdate(edit);
        uiControlRedraw(control, 1);
    }
    else if(event->Keyboard.Key == VK_RIGHT)
    {
        char **linePtr = (char **)vecGet(edit->Lines, edit->CursorRow);
        if(linePtr)
        {
            char *line = *linePtr;
            size_t lineLen = strlen(line);
            if(edit->CursorColumn < lineLen)
                ++edit->CursorColumn;
            else
            {
                int lineCount = vecSize(edit->Lines);
                if(edit->CursorRow < (lineCount - 1))
                {
                    edit->CursorColumn = 0;
                    ++edit->CursorRow;
                }
            }
        }
        textEditUpdate(edit);
        uiControlRedraw(control, 1);
    }
    else if(event->Keyboard.Key == VK_PRIOR)
    {
        int viewLines = uiScrollbarGetZoom(edit->VScroll) / fntGetPixelAscender(edit->Control.Font);
        edit->CursorRow = max(0, edit->CursorRow - viewLines);
        textEditUpdate(edit);
        uiControlRedraw(control, 1);
    }
    else if(event->Keyboard.Key == VK_NEXT)
    {
        int viewLines = uiScrollbarGetZoom(edit->VScroll) / fntGetPixelAscender(edit->Control.Font);
        edit->CursorRow = min(vecSize(edit->Lines) - 1, edit->CursorRow + viewLines);
        textEditUpdate(edit);
        uiControlRedraw(control, 1);
    }
    else if(event->Keyboard.Key == VK_HOME)
    {
        edit->CursorColumn = 0;
        if(edit->Ctrl)
            edit->CursorRow = 0;
        textEditUpdate(edit);
        uiControlRedraw(control, 1);
    }
    else if(event->Keyboard.Key == VK_END)
    {
        char **linePtr = (char **)vecGet(edit->Lines, edit->CursorRow);
        if(linePtr)
        {
            char *line = *linePtr;
            edit->CursorColumn = line ? strlen(line) : 0;
            if(edit->Ctrl)
            {
                int lineCount = vecSize(edit->Lines);
                edit->CursorRow = lineCount - 1;
            }
        } else edit->CursorColumn = 0;
        textEditUpdate(edit);
        uiControlRedraw(control, 1);
    }
    else if(event->Keyboard.Key == VK_RETURN && edit->Editable)
    {
        chr = 0;
        char **linePtr = (char **)vecGet(edit->Lines, edit->CursorRow);
        if(linePtr)
        {
            char *line = *linePtr;
            size_t lineLen = strlen(line);
            int lineCut = min(lineLen, edit->CursorColumn);
            uiTextEditInsertLineAfter(edit, line + lineCut, edit->CursorRow++);
            line[lineCut] = 0;
        }
        else
        {
            uiTextEditAppendLine(edit, "");
            edit->CursorRow = 0;
        }
        edit->CursorColumn = 0;
        textEditUpdate(edit);
        uiControlRedraw(control, 1);
    }
    else if(event->Keyboard.Key == VK_BACK && edit->Editable && edit->CursorColumn <= 0 && edit->CursorRow > 0)
    {
        chr = 0;
        char **prevLinePtr = (char **)vecGet(edit->Lines, edit->CursorRow - 1);
        char *prevLine = prevLinePtr ? *prevLinePtr : "";
        char **thisLinePtr = (char **)vecGet(edit->Lines, edit->CursorRow);
        char *thisLine = thisLinePtr ? *thisLinePtr : "";

        size_t prevLen = strlen(prevLine);
        size_t thisLen = strlen(thisLine);
        size_t newLen = prevLen + thisLen;
        char *newLine = (char *)malloc(newLen + 1);
        snprintf(newLine, newLen + 1, "%s%s", prevLine, thisLine);
        uiTextEditSetLine(edit, newLine, edit->CursorRow - 1);
        uiTextEditRemoveLineAt(edit, edit->CursorRow);
        --edit->CursorRow;
        edit->CursorColumn = prevLen;

        textEditUpdate(edit);
        uiControlRedraw(control, 1);
    }
    else if(event->Keyboard.Key == VK_DELETE && edit->Editable)
    {
        size_t lineCount = vecSize(edit->Lines);
        char **thisLinePtr = (char **)vecGet(edit->Lines, edit->CursorRow);
        if(thisLinePtr)
        {
            char *thisLine = *thisLinePtr;
            size_t thisLen = strlen(thisLine);
            if(edit->CursorColumn >= thisLen && edit->CursorRow < (lineCount - 1))
            {
                chr = 0;
                char **nextLinePtr = (char **)vecGet(edit->Lines, edit->CursorRow + 1);
                if(nextLinePtr)
                {
                    char *nextLine = *nextLinePtr;
                    size_t nextLen = strlen(nextLine);
                    size_t newLen = thisLen + nextLen;
                    char *newLine = (char *)malloc(newLen + 1);
                    snprintf(newLine, newLen + 1, "%s%s", thisLine, nextLine);
                    uiTextEditSetLine(edit, newLine, edit->CursorRow);
                    uiTextEditRemoveLineAt(edit, edit->CursorRow + 1);
                }
                uiControlRedraw(control, 1);
            }
        }
    }
    else if(event->Keyboard.Key == VK_TAB)
        chr = '\t';

    if(!chr || !edit->Editable || edit->Ctrl) return;

    fntFont_t *font = edit->Control.Font;
    char **linePtr = (char **)vecGet(edit->Lines, edit->CursorRow);
    if(!linePtr)
    {
        uiTextEditAppendLine(edit, "");
        linePtr = (char **)vecGet(edit->Lines, edit->CursorRow);
    }

    if(linePtr)
    {
        char *line = *linePtr;
        char *newLine = stringInsert(line, &edit->CursorColumn, chr);
        if(line != newLine) vecSet(edit->Lines, edit->CursorRow, &newLine);
        int newWidth = fntMeasureString(font, newLine, -1);
        if(newWidth < edit->MaxLineWidth)
            recalcMaxWidth(edit);
        else edit->MaxLineWidth = newWidth;
    }
    textEditUpdate(edit);
    uiControlRedraw(control, 1);
}

static void textPreKeyRelease(uiControl_t *control, wmEvent_t *event)
{
    if(!control || !event) return;
    uiTextEdit_t *edit = (uiTextEdit_t *)control->Parent;
    if(event->Keyboard.Key == VK_LSHIFT || event->Keyboard.Key == VK_RSHIFT)
        edit->Shift = UI_FALSE;
    else if(event->Keyboard.Key == VK_LCONTROL || event->Keyboard.Key == VK_RCONTROL)
        edit->Ctrl = UI_FALSE;
}

uiTextEdit_t *uiTextEditCreate(uiControl_t *parent, int x, int y, int width, int height)
{
    uiTextEdit_t *edit = (uiTextEdit_t *)uiControlCreate(parent, sizeof(uiTextEdit_t), NULL, x, y, width, height, NULL);
    if(!edit) return NULL;
    edit->Lines = vecCreate(sizeof(char *), 16, 16, 1 << 31);
    if(!edit->Lines)
    {
        uiTextEditDelete(edit);
        return NULL;
    }
    edit->Control.OnGotFocus = editOnGotFocus;
    edit->Control.CanHaveFocus = UI_TRUE;

    edit->TextContainer = uiControlCreate(&edit->Control, 0, NULL, 0, 0, edit->Control.Rectangle.Width - 16, edit->Control.Rectangle.Height - 16, NULL);
    edit->TextContainer->OnPaint = textContainerOnPaint;
    edit->TextContainer->BackColor = pmColorWhite;
    edit->TextContainer->BorderStyle = UI_BORDER_SUNKEN;
    edit->TextContainer->CanHaveFocus = UI_TRUE;
    edit->TextContainer->PreKeyPress = textPreKeyPress;
    edit->TextContainer->PreKeyRelease = textPreKeyRelease;
    edit->TextContainer->OnCaretPaint = onCaretPaint;
    edit->TextContainer->OnCaretClear = onCaretClear;

    int viewWidth = edit->Control.Rectangle.Width - (edit->TextContainer->BorderStyle == UI_BORDER_NONE ? 0 : 2);
    int viewHeight = edit->Control.Rectangle.Height - (edit->TextContainer->BorderStyle == UI_BORDER_NONE ? 0 : 2);
    edit->HScroll = uiScrollbarCreate(&edit->Control, 0, edit->Control.Rectangle.Height - 16, edit->Control.Rectangle.Width - 16, 16, 1, 0, viewWidth, 0, viewWidth);
    edit->VScroll = uiScrollbarCreate(&edit->Control, edit->Control.Rectangle.Width - 16, 0, 16, edit->Control.Rectangle.Height - 16, 0, 0, viewHeight, 0, viewHeight);

    uiScrollbarSetOnChangePosition(edit->HScroll, scrollChangePosition);
    uiScrollbarSetOnChangePosition(edit->VScroll, scrollChangePosition);

    edit->CaretX = 1; edit->CaretY = 1;
    edit->CaretH = fntGetPixelAscender(edit->TextContainer->Font) - fntGetPixelDescender(edit->TextContainer->Font);

    return edit;
}

void uiTextEditDelete(uiTextEdit_t *edit)
{
    if(!edit) return;
    if(edit->Lines)
    {
        unsigned lineCount = vecSize(edit->Lines);
        for(unsigned i = 0; i < lineCount; ++i)
        {
            char **linePtr = (char **)vecGet(edit->Lines, i);
            if(!linePtr) continue;
            char *line = *linePtr;
            if(!line) continue;
            free(line);
        }
        vecDelete(edit->Lines);
    }
    uiControlDelete((uiControl_t *)edit);
}

void uiTextEditSetEditable(uiTextEdit_t *edit, uiBool_t value)
{
    if(!edit) return;
    edit->Editable = value;
}

uiBool_t uiTextEditGetEditable(uiTextEdit_t *edit)
{
    if(!edit) return UI_FALSE;
    return edit->Editable;
}

int uiTextEditClear(uiTextEdit_t *edit)
{
    if(!edit) return -EINVAL;
    unsigned lineCount = vecSize(edit->Lines);
    for(unsigned i = 0; i < lineCount; ++i)
    {
        char **linePtr = (char **)vecGet(edit->Lines, i);
        if(!linePtr) continue;
        char *line = *linePtr;
        if(!line) continue;
        free(line);
    }
    vecClear(edit->Lines);
    edit->MaxLineWidth = 0;
    textEditUpdate(edit);
    return 0;
}

int uiTextEditAppendLine(uiTextEdit_t *edit, const char *line)
{
    if(!edit) return -EINVAL;
    char *l = strdup(line);
    if(!l) return -errno;
    int res = vecAppend(edit->Lines, &l);
    if(res < 0)
    {
        free(l);
        return res;
    }
    int lineWidth = fntMeasureString(edit->Control.Font, l, -1);
    edit->MaxLineWidth = max(lineWidth, edit->MaxLineWidth);
    textEditUpdate(edit);
    return 0;
}

int uiTextEditInsertLineAfter(uiTextEdit_t *edit, const char *line, int idx)
{
    if(!edit) return -EINVAL;
    char *l = strdup(line);
    if(!l) return -errno;
    int res = vecInsertAfter(edit->Lines, idx, &l);
    if(res < 0)
    {
        free(l);
        return res;
    }
    int lineWidth = fntMeasureString(edit->Control.Font, l, -1);
    edit->MaxLineWidth = max(lineWidth, edit->MaxLineWidth);
    textEditUpdate(edit);
    return 0;
}

int uiTextEditInsertLineBefore(uiTextEdit_t *edit, const char *line, int idx)
{
    if(!edit) return -EINVAL;
    char *l = strdup(line);
    if(!l) return -errno;
    int res = vecInsertBefore(edit->Lines, idx, &l);
    if(res < 0)
    {
        free(l);
        return res;
    }
    int lineWidth = fntMeasureString(edit->Control.Font, l, -1);
    edit->MaxLineWidth = max(lineWidth, edit->MaxLineWidth);
    textEditUpdate(edit);
    return 0;
}

int uiTextEditRemoveLineAt(uiTextEdit_t *edit, int idx)
{
    if(!edit) return -EINVAL;
    char *line = *(char **)vecGet(edit->Lines, idx);
    if(!line) return -ENOENT;
    int res = vecRemoveAt(edit->Lines, idx);
    if(res < 0) return res;
    free(line);
    recalcMaxWidth(edit);
    textEditUpdate(edit);
    return 0;
}

int uiTextEditSetLine(uiTextEdit_t *edit, const char *line, int idx)
{
    if(!edit || !line) return -EINVAL;
    char **lPtr = (char **)vecGet(edit->Lines, idx);
    if(!lPtr) return -EINVAL;

    char *l = *lPtr;
    if(!l) return -ENOENT;
    char *newLine = strdup(line);
    if(!newLine) return -errno;
    vecSet(edit->Lines, idx, &newLine);
    free(l);
    int lineWidth = fntMeasureString(edit->Control.Font, l, -1);
    edit->MaxLineWidth = max(lineWidth, edit->MaxLineWidth);
    textEditUpdate(edit);
    return 0;
}

char *uiTextEditGetLine(uiTextEdit_t *edit, int idx)
{
    if(!edit)
    {
        errno = EINVAL;
        return NULL;
    }
    char **linePtr = (char **)vecGet(edit->Lines, idx);
    if(!linePtr)
    {
        errno = ENOENT;
        return NULL;
    }
    return *linePtr;
}

int uiTextEditGetLineCount(uiTextEdit_t *edit)
{
    if(!edit) return -EINVAL;
    return vecSize(edit->Lines);
}
