#include <stdlib.h>
#include <woot/uilabel.h>

struct uiLabel
{
    uiControl_t Control;
};

uiLabel_t *uiLabelCreate(uiControl_t *parent, int x, int y, int width, int height, const char *text, uiEventHandler onCreate)
{
    uiLabel_t *control = (uiLabel_t *)uiControlCreate(parent, sizeof(uiLabel_t), NULL, x, y, width, height, text, onCreate);
    if(!control) return NULL;
    return control;
}

void uiLabelDelete(uiLabel_t *control)
{
    uiControlDelete((uiControl_t *)control);
}
