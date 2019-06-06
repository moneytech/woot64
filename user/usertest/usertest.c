#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syscalls/syscalls.h>
#include <time.h>
#include <unistd.h>
#include <woot/font.h>
#include <woot/input.h>
#include <woot/ipc.h>
#include <woot/pixmap.h>
#include <woot/rpc.h>
#include <woot/thread.h>
#include <woot/ui.h>
#include <woot/uibutton.h>
#include <woot/uilabel.h>
#include <woot/uilineedit.h>
#include <woot/uiscrollbar.h>
#include <woot/uislider.h>
#include <woot/uitextedit.h>
#include <woot/uitoolbar.h>
#include <woot/video.h>
#include <woot/wm.h>

pmPixMap_t *pm = NULL;
wmWindow_t *window = NULL;

void btnClick(uiControl_t *sender, wmEvent_t *event)
{
    uiControl_t *rootControl = wmGetRootControl(window);
    pmColor_t color = pmColorFromRGB(rand(), rand(), rand());
    uiControlSetBackColor(rootControl, color);
    uiControlRedraw(rootControl, 1);
    wmRedrawWindow(window);
}

void sldChange(uiSlider_t *sender)
{
    uiLineEdit_t *edit = uiControlGetContext((uiControl_t *)sender);
    char text[32];
    snprintf(text, sizeof(text), "slider value: %d", uiSliderGetValue(sender));
    uiControlSetText((uiControl_t *)edit, text);
}

void editAccept(uiLineEdit_t *sender)
{
    wmSetWindowTitle(window, uiControlGetText((uiControl_t *)sender));
}

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    int res = wmInitialize(WM_INITIALIZE_NONE);
    const char *wmServer = wmGetServer();
    if(res < 0)
    {
        printf("[usertest] wmInitialize() failed\n");
        return -1;
    }
    else printf("[usertest] Window manager server: '%s'\n", wmServer);

    window = wmCreateWindow(100, 200, 300, 350, WM_CWF_DEFAULT);
    if(!window) return -errno;
    wmSetWindowTitle(window, "Test window");

    pm = wmGetPixMap(window);
    if(!pm)
    {
        wmDeleteWindow(window);
        return -errno;
    }

    pmPixMap_t *fileIcon = pmLoadPNG("/data/common/icons/file.png");
    pmPixMap_t *clockIcon = pmLoadPNG("/clock_small.png");

    uiControl_t *rootControl = wmGetRootControl(window);
    //uiControlSetBackColor(rootControl, pmColorTransparent);
    //uiControlSetBackColor(rootControl, pmColorRed);
    uiControl_t *lbl = (uiControl_t *)uiLabelCreate(rootControl, 20, 1, pm->Contents.Width - 41, 24, "Date and time");
    uiControl_t *btn = (uiControl_t *)uiButtonCreate(rootControl, (pm->Contents.Width - 100) / 2, 30, 100, 80, "Do something");
    uiLineEdit_t *edit = uiLineEditCreate(rootControl, (pm->Contents.Width - 120) / 2, 120, 120, 30, "Trolololo");
    uiSlider_t *sld = uiSliderCreate(rootControl, (pm->Contents.Width - 120) / 2, 150, 120, 24, 1, 0, 100, 25);
    uiSlider_t *sld2 = uiSliderCreate(rootControl, 1, 40, 24, 130, 0, 0, 100, 25);
    uiScrollbar_t *scroll = uiScrollbarCreate(rootControl, 283, 1, 16, 182, 0, 0, 99, 0, 5);
    uiScrollbar_t *scroll2 = uiScrollbarCreate(rootControl, 1, 183, 283, 16, 1, 0, 99, 88, 40);
    uiTextEdit_t *edit2 = uiTextEditCreate(rootControl, 10, 240, pm->Contents.Width - 20, 100);

    uiToolbar_t *bar = uiToolbarCreate(rootControl, 1, 199, pm->Contents.Width - 2, 30, UI_HORIZONTAL);
    for(int i = 0; i < 4; ++i)
        uiButtonCreate((uiControl_t *)bar, 10, 0, 64, 24, "Abcd");
    uiToolbarSetChildSpacing(bar, 4);
    uiControlRecalcRects((uiControl_t *)bar);


    uiControlSetBorderColor(rootControl, pmColorWhite);
    uiControlSetBorderStyle(rootControl, UI_BORDER_RAISED);

    uiControlSetTextColor(lbl, pmColorWhite);
    uiControlSetBackColor(lbl, pmColorBrighten(wmGetColor(WM_COLOR_BACKGROUND), -0.05f));
    uiControlSetTextIconSeparation(lbl, 4);
    uiControlSetIconPosition(lbl, UI_LEFT);
    uiControlSetIcon(lbl, clockIcon);

    uiControlSetIcon(btn, fileIcon);
    uiControlSetIconPosition(btn, UI_OVER);
    uiControlSetTextIconSeparation(btn, 2);
    uiControlSetOnMousePress(btn, btnClick);

    uiSliderSetOnValueChange(sld, sldChange);
    uiControlSetContext((uiControl_t *)sld, edit);

    uiLineEditSetOnAcceptInput(edit, editAccept);

    uiControlSetFont((uiControl_t *)edit2, wmGetFont(WM_FONT_MONO));
    uiTextEditSetEditable(edit2, UI_TRUE);
    for(int i = 0; i < 100; ++i)
    {
        char buf[64];
        snprintf(buf, sizeof(buf), "This is line number %d", i + 1);
        uiTextEditAppendLine(edit2, buf);
    }

    uiControlRedraw(rootControl, 1);

    srand(time(NULL));
    ipcMessage_t msg;
    for(int i = 0;; ++i)
    {
        int quit = 0;

        int msgTimeout = 400;
        while(ipcGetMessage(&msg, msgTimeout) >= 0)
        {
            msgTimeout = 400;
            ipcProcessMessage(&msg);
            if(msg.Number == MSG_QUIT)
            {
                quit = 1;
                break;
            }
            else if(msg.Number == MSG_WM_EVENT)
            {
                wmEvent_t *event = (wmEvent_t *)msg.Data;
                if(event->WindowId == wmGetWindowId(window))
                    wmProcessEvent(window, event);
                if(event->Type == WM_EVT_CLOSE)
                {
                    quit = 1;
                    break;
                }
                else if(event->Type == WM_EVT_KEYBOARD)
                {
                    if(!(event->Keyboard.Flags & WM_EVT_KB_RELEASED))
                    {
                        if(event->Keyboard.Key == VK_ESCAPE)
                        {
                            quit = 1;
                            break;
                        }
                    }
                }
            }
        }
        if(quit) break;

        char buf[64];
        time_t ct = time(NULL);
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", gmtime(&ct));
        uiControlSetText((uiControl_t *)lbl, buf);
    }

    printf("[usertest] Closing usertest\n");

    wmDeleteWindow(window);

    return 0;
}
