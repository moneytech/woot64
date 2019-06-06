#pragma once

#include <woot/ui.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct uiTextEdit uiTextEdit_t;

uiTextEdit_t *uiTextEditCreate(uiControl_t *parent, int x, int y, int width, int height);
void uiTextEditDelete(uiTextEdit_t *edit);
void uiTextEditSetEditable(uiTextEdit_t *edit, uiBool_t value);
uiBool_t uiTextEditGetEditable(uiTextEdit_t *edit);
int uiTextEditClear(uiTextEdit_t *edit);
int uiTextEditAppendLine(uiTextEdit_t *edit, const char *line);
int uiTextEditInsertLineAfter(uiTextEdit_t *edit, const char *line, int idx);
int uiTextEditInsertLineBefore(uiTextEdit_t *edit, const char *line, int idx);
int uiTextEditRemoveLineAt(uiTextEdit_t *edit, int idx);
int uiTextEditSetLine(uiTextEdit_t *edit, const char *line, int idx);
char *uiTextEditGetLine(uiTextEdit_t *edit, int idx);
int uiTextEditGetLineCount(uiTextEdit_t *edit);

#ifdef __cplusplus
}
#endif // __cplusplus
