#pragma once

#include <woot/ui.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct uiLineEdit uiLineEdit_t;

uiLineEdit_t *uiLineEditCreate(uiControl_t *parent, int x, int y, int width, int height, const char *text, uiEventHandler onCreate);
void uiLineEditDelete(uiLineEdit_t *control);

#ifdef __cplusplus
}
#endif // __cplusplus
