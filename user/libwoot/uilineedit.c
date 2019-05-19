#include <stdlib.h>
#include <string.h>
#include <woot/uilineedit.h>
#include <woot/wm.h>

struct uiLineEdit
{
    uiControl_t Control;
    int CursorPosition;
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
        return str;
    }
    str = realloc(str, len + 2);
    memmove(str + *pos + 1, str + *pos, len - *pos + 1);
    str[*pos++] = chr;
    return str;
}

static void lineEditPreKeyPress(uiControl_t *control, wmEvent_t *event)
{
    if(!control || !event) return;
    int chr = event->Keyboard.Character;
    if(!chr) return;
    uiLineEdit_t *edit = (uiLineEdit_t *)control;
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

    control->CursorPosition = -1; // -1 means end of text
    return control;
}

void uiLineEditDelete(uiLineEdit_t *control)
{
    uiControlDelete((uiControl_t *)control);
}
