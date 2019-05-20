#pragma once

#include <woot/ui.h>

typedef struct uiScrollbar uiScrollbar_t;

typedef void (*uiScrollbarChangePositionHandler)(uiScrollbar_t *sender);

uiScrollbar_t *uiScrollbarCreate(uiControl_t *parent, int x, int y, int width, int height, int horizontal, int minPos, int maxPos, int pos, int zoom);
void uiScrollbarDelete(uiScrollbar_t *control);
void uiScrollbarSetPosition(uiScrollbar_t *control, int position);
int uiScrollbarGetPosition(uiScrollbar_t *control);
void uiScrollbarSetZoom(uiScrollbar_t *control, int zoom);
int uiScrollbarGetZoom(uiScrollbar_t *control);
void uiScrollbarSetMinPosition(uiScrollbar_t *control, int position);
int uiScrollbarGetMinPosition(uiScrollbar_t *control);
void uiScrollbarSetMaxPosition(uiScrollbar_t *control, int position);
int uiScrollbarGetMaxPosition(uiScrollbar_t *control);

