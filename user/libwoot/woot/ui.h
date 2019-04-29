#ifndef UI_H
#define UI_H

#include <woot/font.h>
#include <woot/pixmap.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// horizontal element alignment
#define UI_HALIGN_LEFT      0
#define UI_HALIGN_CENTER    1
#define UI_HALIGN_RIGHT     2

// vertical element alignment
#define UI_VALIGN_TOP       0
#define UI_VALIGN_MIDDLE    1
#define UI_VALIGN_BOTTOM    2

// border styles
#define UI_BORDER_NONE      0
#define UI_BORDER_SIMPLE    1
#define UI_BORDER_RAISED    2
#define UI_BORDER_SUNKEN    3

// control visibility
#define UI_HIDDEN           0
#define UI_VISIBLE          1

// icon position
#define UI_ICON_BEHIND      0
#define UI_ICON_OVER        1
#define UI_ICON_BELOW       2
#define UI_ICON_LEFT        3
#define UI_ICON_RIGHT       4

typedef struct wmEvent wmEvent_t;

typedef struct uiControl uiControl_t;
typedef struct uiLabel uiLabel_t;
typedef struct uiButton uiButton_t;
typedef struct uiLineEdit uiLineEdit_t;
typedef struct uiSlider uiSlider_t;

typedef void (*uiEventHandler)(uiControl_t *sender);
typedef void (*uiWMEventHandler)(uiControl_t *sender, wmEvent_t *event);

uiControl_t *uiControlCreate(uiControl_t *parent, size_t structSize, pmPixMap_t *parentPixMap, int x, int y, int width, int height, const char *text, uiEventHandler onCreate);
void uiControlDelete(uiControl_t *control);
void uiControlRedraw(uiControl_t *control);
pmPixMap_t *uiControlGetPixMap(uiControl_t *control);
void *uiControlGetContext(uiControl_t *control);
void uiControlSetContext(uiControl_t *control, void *context);
void uiControlSetVisibility(uiControl_t *control, int visibility);
char *uiControlGetText(uiControl_t *control);
void uiControlSetText(uiControl_t *control, const char *text);
void uiControlSetIcon(uiControl_t *control, pmPixMap_t *icon);
int uiControlProcessEvent(uiControl_t *control, wmEvent_t *event);
void uiControlSetTextColor(uiControl_t *control, pmColor_t color);
void uiControlSetBackColor(uiControl_t *control, pmColor_t color);
void uiControlSetBorderColor(uiControl_t *control, pmColor_t color);
void uiControlSetTextHAlign(uiControl_t *control, int align);
void uiControlSetTextVAlign(uiControl_t *control, int align);
void uiControlSetBorderStyle(uiControl_t *control, int style);
void uiControlSetMarginSize(uiControl_t *control, int size);
void uiControlSetIconPosition(uiControl_t *control, int position);
void uiControlSetTextIconSeparation(uiControl_t *control, int separation);
void uiControlSetOnPaint(uiControl_t *control, uiEventHandler handler);
void uiControlSetOnMousePress(uiControl_t *control, uiWMEventHandler handler);
void uiControlSetOnMouseRelease(uiControl_t *control, uiWMEventHandler handler);
void uiControlSetOnMouseMove(uiControl_t *control, uiWMEventHandler handler);

uiLabel_t *uiLabelCreate(uiControl_t *parent, int x, int y, int width, int height, const char *text, uiEventHandler onCreate);
void uiLabelDelete(uiLabel_t *control);

uiButton_t *uiButtonCreate(uiControl_t *parent, int x, int y, int width, int height, const char *text, uiEventHandler onCreate);
void uiButtonDelete(uiButton_t *control);

uiLineEdit_t *uiLineEditCreate(uiControl_t *parent, int x, int y, int width, int height, const char *text, uiEventHandler onCreate);
void uiLineEditDelete(uiLineEdit_t *control);

uiSlider_t *uiSliderCreate(uiControl_t *parent, int x, int y, int width, int height, int horizontal, int minVal, int maxVal, int val);
void uiSliderSetValue(uiSlider_t *control, int value);
int uiSliderGetValue(uiSlider_t *control);
void uiSliderSetMinValue(uiSlider_t *control, int value);
int uiSliderGetMinValue(uiSlider_t *control);
void uiSliderSetMaxValue(uiSlider_t *control, int value);
int uiSliderGetMaxValue(uiSlider_t *control);
void uiSliderDelete(uiSlider_t *control);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // UI_H
