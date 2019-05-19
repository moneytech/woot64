#include <stdlib.h>
#include <woot/uibutton.h>

struct uiButton
{
    uiControl_t Control;
};

uiButton_t *uiButtonCreate(uiControl_t *parent, int x, int y, int width, int height, const char *text, uiEventHandler onCreate)
{
    uiButton_t *control = (uiButton_t *)uiControlCreate(parent, sizeof(uiButton_t), NULL, x, y, width, height, text, onCreate);
    if(!control) return NULL;
    control->Control.CanHaveFocus = 1;
    control->Control.BorderStyle = UI_BORDER_RAISED;
    return control;
}

void uiButtonDelete(uiButton_t *control)
{
    uiControlDelete((uiControl_t *)control);
}
