#pragma once

#include <woot/ui.h>

typedef struct uiButton uiButton_t;

uiButton_t *uiButtonCreate(uiControl_t *parent, int x, int y, int width, int height, const char *text, uiEventHandler onCreate);
void uiButtonDelete(uiButton_t *control);
