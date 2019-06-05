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
        uiScrollbarSetPosition(edit->VScroll, lineHeight * (edit->CursorRow + 1) - viewHeight);
        redrawScroll = 1;
    }

    char *line = *(char **)vecGet(edit->Lines, edit->CursorRow);
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

        if(i == edit->CursorRow && sender->HasFocus)
        {
            int curX = fntMeasureString(font, line, edit->CursorColumn);
            pmLine(pm, 1 + xoffs + curX, y, 1 + xoffs + curX, y + lineHeight - descender, textColor);
        }
    }

    textContainerDrawBorder(sender);
}

static void vScrollChangePosition(uiScrollbar_t *scroll)
{
    //printf("%d\n", uiScrollbarGetPosition(scroll));
    uiTextEdit_t *edit = (uiTextEdit_t *)uiControlGetParent((uiControl_t *)scroll);
    uiControlRedraw(edit->TextContainer, 1);
}

static void textPreKeyPress(uiControl_t *control, wmEvent_t *event)
{
    if(!control || !event) return;
    uiTextEdit_t *edit = (uiTextEdit_t *)control->Parent;
    int chr = event->Keyboard.Character;
    if(event->Keyboard.Key == VK_UP)
    {
        if(edit->CursorRow > 0)
            --edit->CursorRow;
        textEditUpdate(edit);
        uiControlRedraw(control, 1);
    }
    else if(event->Keyboard.Key == VK_DOWN)
    {
        int lineCount = vecSize(edit->Lines);
        if(edit->CursorRow < lineCount)
            ++edit->CursorRow;
        textEditUpdate(edit);
        uiControlRedraw(control, 1);
    }
    else if(event->Keyboard.Key == VK_LEFT)
    {
        if(edit->CursorColumn > 0)
            --edit->CursorColumn;
        textEditUpdate(edit);
        uiControlRedraw(control, 1);
    }
    else if(event->Keyboard.Key == VK_RIGHT)
    {
        char *line = *(char **)vecGet(edit->Lines, edit->CursorRow);
        if(line)
        {
            size_t lineLen = strlen(line);
            if(edit->CursorColumn < lineLen)
                ++edit->CursorColumn;
            textEditUpdate(edit);
            uiControlRedraw(control, 1);
        }
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
        edit->CursorRow = min(vecSize(edit->Lines), edit->CursorRow + viewLines);
        textEditUpdate(edit);
        uiControlRedraw(control, 1);
    }

    if(!chr) return;

    fntFont_t *font = edit->Control.Font;
    char *line = *(char **)vecGet(edit->Lines, edit->CursorRow);
    char *newLine = stringInsert(line, &edit->CursorColumn, chr);
    if(line != newLine) vecSet(edit->Lines, edit->CursorRow, &newLine);
    int newWidth = fntMeasureString(font, newLine, -1);
    if(newWidth < edit->MaxLineWidth)
    {   // we have to completely recalculate max text width
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
    else edit->MaxLineWidth = newWidth;
    textEditUpdate(edit);
    uiControlRedraw(control, 1);
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
    edit->TextContainer = uiControlCreate(&edit->Control, 0, NULL, 0, 0, edit->Control.Rectangle.Width - 16, edit->Control.Rectangle.Height - 16, NULL);
    edit->TextContainer->OnPaint = textContainerOnPaint;
    edit->TextContainer->BackColor = pmColorWhite;
    edit->TextContainer->BorderStyle = UI_BORDER_SUNKEN;
    edit->TextContainer->CanHaveFocus = UI_TRUE;
    edit->TextContainer->PreKeyPress = textPreKeyPress;

    int viewWidth = edit->Control.Rectangle.Width - (edit->TextContainer->BorderStyle == UI_BORDER_NONE ? 0 : 2);
    int viewHeight = edit->Control.Rectangle.Height - (edit->TextContainer->BorderStyle == UI_BORDER_NONE ? 0 : 2);
    edit->HScroll = uiScrollbarCreate(&edit->Control, 0, edit->Control.Rectangle.Height - 16, edit->Control.Rectangle.Width - 16, 16, 1, 0, viewWidth, 0, viewWidth);
    edit->VScroll = uiScrollbarCreate(&edit->Control, edit->Control.Rectangle.Width - 16, 0, 16, edit->Control.Rectangle.Height - 16, 0, 0, viewHeight, 0, viewHeight);

    uiScrollbarSetOnChangePosition(edit->HScroll, vScrollChangePosition);
    uiScrollbarSetOnChangePosition(edit->VScroll, vScrollChangePosition);

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
            char *line = *(char **)vecGet(edit->Lines, i);
            if(!line) continue;
            free(line);
        }
        vecDelete(edit->Lines);
    }
    uiControlDelete((uiControl_t *)edit);
}

void uiTextEditClear(uiTextEdit_t *edit)
{
    unsigned lineCount = vecSize(edit->Lines);
    for(unsigned i = 0; i < lineCount; ++i)
    {
        char *line = *(char **)vecGet(edit->Lines, i);
        if(!line) continue;
        free(line);
    }
    vecClear(edit->Lines);
}

void uiTextEditAppendLine(uiTextEdit_t *edit, const char *line)
{
    char *l = strdup(line);
    vecAppend(edit->Lines, &l);
    int lineWidth = fntMeasureString(edit->Control.Font, l, -1);
    edit->MaxLineWidth = max(lineWidth, edit->MaxLineWidth);
    textEditUpdate(edit);
}
