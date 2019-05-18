#include <stdlib.h>
#include <woot/uislider.h>
#include <woot/vkeys.h>
#include <woot/wm.h>

struct uiSlider
{
    uiControl_t Control;
    int Horizontal;
    int MinValue;
    int MaxValue;
    int Value;

    uiSliderChangeValueHandler OnValueChange;

    int dragging;
};

static int mapValue(int imin, int imax, int omin, int omax, int val)
{
    return (float)(val - imin) / (imax - imin) * (omax - omin) + omin;
}

static void sliderUpdate(uiSlider_t *slider, wmEvent_t *event)
{
    if(event)
    {
        rcRectangle_t rect = pmGetRectangle(slider->Control.PixMap);

        int val = slider->Value;

        if(slider->Horizontal)
            val = mapValue(4, rect.Width - 8, slider->MinValue, slider->MaxValue, event->Mouse.Coords[0]);
        else val = mapValue(4, rect.Height - 8, slider->MinValue, slider->MaxValue, rect.Height - event->Mouse.Coords[1]);

        if(val < slider->MinValue) val = slider->MinValue;
        else if(val > slider->MaxValue) val = slider->MaxValue;
        slider->Value = val;
    }
    if(slider->OnValueChange)
        slider->OnValueChange(slider);
    uiControlRedraw((uiControl_t *)slider, 1);
}

static void sliderPaint(uiControl_t *control)
{
    if(!control) return;
    uiSlider_t *slider = (uiSlider_t *)control;
    rcRectangle_t rect = pmGetRectangle(slider->Control.PixMap);
    pmPixMap_t *pm = control->PixMap;
    rect.X = rect.Y = 0;
    if(slider->Horizontal)
    {
        int cy = rect.Height / 2;
        pmDrawFrame(pm, 4, cy - 1, rect.Width - 8, 2, 1);
        int x = mapValue(slider->MinValue, slider->MaxValue, 4, rect.Width - 4, slider->Value);
        pmFillRectangle(pm, x - 2, 5, 4, rect.Height - 10, pmColorGray);
        pmDrawFrame(pm, x - 3, 4, 6, rect.Height - 8, 0);
    }
    else
    {
        int cx = rect.Width / 2;
        pmDrawFrame(pm, cx - 1, 4, 2, rect.Height - 8, 1);
        int y = mapValue(slider->MaxValue, slider->MinValue, 4, rect.Height - 4, slider->Value);
        pmFillRectangle(pm, 5, y - 2, rect.Width - 10, 4, pmColorGray);
        pmDrawFrame(pm, 4, y - 3, rect.Width - 8, 6, 0);
    }
    if(control->HasFocus)
        pmRectanglePattern(pm, 1, 1, rect.Width - 2, rect.Height - 2, 0x55555555, pmColorBlack);
    pmInvalidateWhole(pm);
}

static void sliderPreMousePress(uiControl_t *control, wmEvent_t *event)
{
    if(!control || !event) return;
    if(!(event->Mouse.ButtonsPressed & 1))
        return;
    uiSlider_t *slider = (uiSlider_t *)control;
    slider->dragging = 1;
    sliderUpdate(slider, event);
}

static void sliderPreMouseMove(uiControl_t *control, wmEvent_t *event)
{
    if(!control || !event) return;
    uiSlider_t *slider = (uiSlider_t *)control;
    if(!(event->Mouse.ButtonsHeld & 1))
        slider->dragging = 0;
    if(!slider->dragging) return;
    sliderUpdate(slider, event);
}

static void sliderPostMouseRelease(uiControl_t *control, wmEvent_t *event)
{
    if(!control || !event) return;
    if(!(event->Mouse.ButtonsReleased & 1))
        return;
    uiSlider_t *slider = (uiSlider_t *)control;
    slider->dragging = 0;
}

static void sliderPreKeyPress(uiControl_t *control, wmEvent_t *event)
{
    if(!control || !event) return;
    uiSlider_t *slider = (uiSlider_t *)control;
    if(event->Keyboard.Key == VK_LEFT || event->Keyboard.Key == VK_DOWN)
        uiSliderSetValue(slider, uiSliderGetValue(slider) - 1);
    else if(event->Keyboard.Key == VK_UP || event->Keyboard.Key == VK_RIGHT)
        uiSliderSetValue(slider, uiSliderGetValue(slider) + 1);
}

uiSlider_t *uiSliderCreate(uiControl_t *parent, int x, int y, int width, int height, int horizontal, int minVal, int maxVal, int val)
{
    struct uiSlider *control = (struct uiSlider *)uiControlCreate(parent, sizeof(struct uiSlider), NULL, x, y, width, height, NULL, NULL);
    if(!control) return NULL;
    control->Control.CanHaveFocus = 1;
    control->Horizontal = horizontal;
    if(maxVal < minVal)
    {
        int t = minVal;
        minVal = maxVal;
        maxVal = t;
    }
    control->MinValue = minVal;
    control->MaxValue = maxVal;
    if(val < minVal) val = minVal;
    else if(val > maxVal) val = maxVal;
    control->Value = val;
    control->Control.OnPaint = sliderPaint;
    control->Control.PreKeyPress = sliderPreKeyPress;
    control->Control.PreMousePress = sliderPreMousePress;
    control->Control.PreMouseMove = sliderPreMouseMove;
    control->Control.PostMouseRelease = sliderPostMouseRelease;
    return control;
}

void uiSliderSetOnValueChange(uiSlider_t *control, uiSliderChangeValueHandler handler)
{
    if(!control) return;
    control->OnValueChange = handler;
}

void uiSliderSetValue(uiSlider_t *control, int value)
{
    if(!control) return;
    if(value < control->MinValue) value = control->MinValue;
    else if(value > control->MaxValue) value = control->MaxValue;
    control->Value = value;
    sliderUpdate(control, NULL);
}

int uiSliderGetValue(uiSlider_t *control)
{
    if(!control) return 0;
    return control->Value;
}

void uiSliderSetMinValue(uiSlider_t *control, int value)
{
    if(!control) return;
    control->MinValue = value;
    if(value > control->MaxValue) control->MaxValue = value;
    uiSliderSetValue(control, control->Value);
}

int uiSliderGetMinValue(uiSlider_t *control)
{
    if(!control) return 0;
    return control->MinValue;
}

void uiSliderSetMaxValue(uiSlider_t *control, int value)
{
    if(!control) return;
    control->MaxValue = value;
    if(value < control->MinValue) control->MinValue = value;
    uiSliderSetValue(control, control->Value);
}

int uiSliderGetMaxValue(uiSlider_t *control)
{
    if(!control) return 0;
    return control->MaxValue;
}

void uiSliderDelete(uiSlider_t *control)
{
    uiControlDelete((uiControl_t *)control);
}
