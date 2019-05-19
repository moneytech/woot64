#pragma once

#include <woot/ui.h>

typedef struct uiLabel uiLabel_t;

uiLabel_t *uiLabelCreate(uiControl_t *parent, int x, int y, int width, int height, const char *text, uiEventHandler onCreate);
void uiLabelDelete(uiLabel_t *control);
