#pragma once

#include <woot/ui.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct uiLineEdit uiLineEdit_t;

typedef void (*uiLineEditAcceptInputHandler)(uiLineEdit_t *sender);

uiLineEdit_t *uiLineEditCreate(uiControl_t *parent, int x, int y, int width, int height, const char *text);
void uiLineEditDelete(uiLineEdit_t *control);
void uiLineEditSetOnAcceptInput(uiLineEdit_t *control, uiLineEditAcceptInputHandler handler);
uiLineEditAcceptInputHandler uiLineEditGetOnAcceptInput(uiLineEdit_t *control);

#ifdef __cplusplus
}
#endif // __cplusplus
