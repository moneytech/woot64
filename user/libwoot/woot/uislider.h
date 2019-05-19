#pragma once

#include <woot/ui.h>

typedef struct uiSlider uiSlider_t;

typedef void (*uiSliderChangeValueHandler)(uiSlider_t *sender);

uiSlider_t *uiSliderCreate(uiControl_t *parent, int x, int y, int width, int height, int horizontal, int minVal, int maxVal, int val);
void uiSliderSetOnValueChange(uiSlider_t *control, uiSliderChangeValueHandler handler);
void uiSliderSetValue(uiSlider_t *control, int value);
int uiSliderGetValue(uiSlider_t *control);
void uiSliderSetMinValue(uiSlider_t *control, int value);
int uiSliderGetMinValue(uiSlider_t *control);
void uiSliderSetMaxValue(uiSlider_t *control, int value);
int uiSliderGetMaxValue(uiSlider_t *control);
void uiSliderDelete(uiSlider_t *control);
