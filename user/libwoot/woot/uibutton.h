#pragma once

#include <woot/ui.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct uiButton uiButton_t;

uiButton_t *uiButtonCreate(uiControl_t *parent, int x, int y, int width, int height, const char *text);
void uiButtonDelete(uiButton_t *control);
void uiButtonSetSelected(uiButton_t *button, uiBool_t value);
uiBool_t uiButtonGetSelected(uiButton_t *button);

#ifdef __cplusplus
}
#endif // __cplusplus
