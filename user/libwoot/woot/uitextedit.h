#pragma once

#include <woot/ui.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct uiTextEdit uiTextEdit_t;

uiTextEdit_t *uiTextEditCreate(uiControl_t *parent, int x, int y, int width, int height);
void uiTextEditDelete(uiTextEdit_t *edit);
void uiTextEditClear(uiTextEdit_t *edit);
void uiTextEditAppendLine(uiTextEdit_t *edit, const char *line);

#ifdef __cplusplus
}
#endif // __cplusplus
