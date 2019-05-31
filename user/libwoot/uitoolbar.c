#include <stdlib.h>
#include <woot/uitoolbar.h>

#undef max
#define max(x, y) ((x) > (y) ? (x) : (y))
#undef min
#define min(x, y) ((x) < (y) ? (x) : (y))

typedef struct uiToolbar
{
    uiControl_t Control;
    uiOrientation_t Orientation;
    int ChildSpacing;
} uiToolbar_t;

static void toolbarRecalcRects(uiControl_t *sender)
{
    uiToolbar_t *bar = (uiToolbar_t *)sender;
    if(bar->Orientation == UI_HORIZONTAL)
    {
        int curPos = bar->ChildSpacing;
        for(uiControl_t *ctrl = sender->Children; ctrl; ctrl = ctrl->Next)
        {
            rcRectangle_t rect =
            {
                curPos,
                (sender->Rectangle.Height - ctrl->Rectangle.Height) / 2,
                ctrl->Rectangle.Width,
                min(ctrl->Rectangle.Height, sender->Rectangle.Height)
            };
            curPos += ctrl->Rectangle.Width + bar->ChildSpacing;
            uiControlSetRect(ctrl, rect);
        }
    }
    else if(bar->Orientation == UI_VERTICAL)
    {
        int curPos = bar->ChildSpacing;
        for(uiControl_t *ctrl = sender->Children; ctrl; ctrl = ctrl->Next)
        {
            rcRectangle_t rect =
            {
                (sender->Rectangle.Width - ctrl->Rectangle.Width) / 2,
                curPos,
                min(ctrl->Rectangle.Width, sender->Rectangle.Width),
                ctrl->Rectangle.Height
            };
            curPos += ctrl->Rectangle.Height + bar->ChildSpacing;
            uiControlSetRect(ctrl, rect);
        }
    }
    uiControlRedraw(sender, 0);
}

uiToolbar_t *uiToolbarCreate(uiControl_t *parent, int x, int y, int w, int h, uiOrientation_t orient)
{
    uiToolbar_t *bar = (uiToolbar_t *)uiControlCreate(parent, sizeof(uiToolbar_t), NULL, x, y, w, h, NULL);
    if(!bar) return NULL;

    bar->Orientation = orient;
    bar->Control.OnRecalcRects = toolbarRecalcRects;

    return bar;
}

void uiToolbarDelete(uiToolbar_t *bar)
{
    uiControlDelete((uiControl_t *)bar);
}

void uiToolbarSetChildSpacing(uiToolbar_t *bar, int spacing)
{
    if(!bar) return;
    bar->ChildSpacing = spacing;
}

int uiToolbarGetChildSpacing(uiToolbar_t *bar)
{
    if(!bar) return 0;
    return bar->ChildSpacing;
}

int uiToolbarGetNextControlPos(uiToolbar_t *bar)
{
    int curPos = bar->ChildSpacing;
    if(bar->Orientation == UI_HORIZONTAL)
    {
        for(uiControl_t *ctrl = bar->Control.Children; ctrl; ctrl = ctrl->Next)
            curPos += ctrl->Rectangle.Width + bar->ChildSpacing;
    }
    else if(bar->Orientation == UI_VERTICAL)
    {
        for(uiControl_t *ctrl = bar->Control.Children; ctrl; ctrl = ctrl->Next)
            curPos += ctrl->Rectangle.Height + bar->ChildSpacing;
    }
    return curPos + bar->ChildSpacing;
}

int uiToolbarGetFreeSpace(uiToolbar_t *bar)
{
    return bar->Control.Rectangle.Width - uiToolbarGetNextControlPos(bar) - bar->ChildSpacing;
}
