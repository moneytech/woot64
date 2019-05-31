#pragma once

#include <woot/ui.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct uiLabel uiLabel_t;

uiLabel_t *uiLabelCreate(uiControl_t *parent, int x, int y, int width, int height, const char *text);
void uiLabelDelete(uiLabel_t *control);

#ifdef __cplusplus
}
#endif // __cplusplus
