#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <woot/uiscrollbar.h>
#include <woot/uitextedit.h>
#include <woot/vector.h>
#include <woot/wm.h>

struct uiTextEdit
{
    uiControl_t Control;
    vecVector_t *Lines;
    uiScrollbar_t *HScroll;
    uiScrollbar_t *VScroll;
    uiControl_t *TextContainer;
};

static void textEditUpdate(uiTextEdit_t *edit)
{
    unsigned lineCount = vecSize(edit->Lines);
    fntFont_t *font = edit->Control.Font;
    float lineHeight = fntGetPixelAscender(font);
    int viewHeight = edit->Control.Rectangle.Height - (edit->TextContainer->BorderStyle == UI_BORDER_NONE ? 0 : 2);
    uiScrollbarSetMaxPosition(edit->VScroll, lineHeight * (lineCount + 1) - fntGetPixelDescender(font) + 1);
    uiScrollbarSetZoom(edit->VScroll, viewHeight);
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
    pmPixMap_t *pm = sender->PixMap;
    pmColor_t textColor = edit->Control.TextColor;
    unsigned lineCount = vecSize(edit->Lines);
    int vPos = uiScrollbarGetPosition(edit->VScroll);
    int evPos = vPos + uiScrollbarGetZoom(edit->VScroll);
    unsigned startLine = vPos / lineHeight;
    unsigned endLine = evPos / lineHeight;
    for(unsigned i = startLine; i < endLine && i < lineCount; ++i)
    {
        char *line = *(char **)vecGet(edit->Lines, i);
        if(!line) continue;

        fntDrawString(font, pm, 2, 1 + i * lineHeight - vPos, line, textColor);
    }

    //drawText(sender);
    textContainerDrawBorder(sender);
}

static void vScrollChangePosition(uiScrollbar_t *scroll)
{
    //printf("%d\n", uiScrollbarGetPosition(scroll));
    uiTextEdit_t *edit = (uiTextEdit_t *)uiControlGetParent((uiControl_t *)scroll);
    uiControlRedraw(edit->TextContainer, 1);
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

    int viewWidth = edit->Control.Rectangle.Width - (edit->TextContainer->BorderStyle == UI_BORDER_NONE ? 0 : 2);
    int viewHeight = edit->Control.Rectangle.Height - (edit->TextContainer->BorderStyle == UI_BORDER_NONE ? 0 : 2);
    edit->HScroll = uiScrollbarCreate(&edit->Control, 0, edit->Control.Rectangle.Height - 16, edit->Control.Rectangle.Width - 16, 16, 1, 0, viewWidth, 0, viewWidth);
    edit->VScroll = uiScrollbarCreate(&edit->Control, edit->Control.Rectangle.Width - 16, 0, 16, edit->Control.Rectangle.Height - 16, 0, 0, viewHeight, 0, viewHeight);

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
    textEditUpdate(edit);
}
