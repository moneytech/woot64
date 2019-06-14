#pragma once

#include <woot/ui.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct uiMenu uiMenu_t;

uiMenu_t *uiMenuCreate();
int uiMenuDelete(uiMenu_t *menu);
int uiMenuShow(uiMenu_t *menu, int x, int y);
int uiMenuHide(uiMenu_t *menu);
int uiMenuAddItem(uiMenu_t *menu, const char *text, pmPixMap_t *icon, uiMenu_t *subMenu);

#ifdef __cplusplus
}
#endif // __cplusplus
