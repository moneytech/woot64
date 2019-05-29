#pragma once

#include <woot/ui.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct uiToolbar uiToolbar_t;

uiToolbar_t *uiToolbarCreate(uiControl_t *parent, int x, int y, int w, int h, uiOrientation_t orient);
void uiToolbarDelete(uiToolbar_t *bar);
void uiToolbarSetChildSpacing(uiToolbar_t *bar, int spacing);
int uiToolbarGetChildSpacing(uiToolbar_t *bar);

#ifdef __cplusplus
}
#endif // __cplusplus
