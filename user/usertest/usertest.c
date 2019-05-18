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
#include <woot/thread.h>
#include <woot/ui.h>
#include <woot/uislider.h>
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

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);

    int res = wmInitialize();
    const char *wmServer = wmGetServer();
    if(res < 0)
    {
        printf("[usertest] wmInitialize() failed\n");
        return -1;
    }
    else printf("[usertest] Window manager server: '%s'\n", wmServer);

    window = wmCreateWindow(100, 200, 300, 200, WM_CWF_DEFAULT);
    if(!window) return -errno;
    wmSetWindowTitle(window, "Test window");

    pm = wmGetPixMap(window);
    if(!pm)
    {
        wmDeleteWindow(window);
        return -errno;
    }

    fntFont_t *fnt = fntLoad("/default.ttf");
    if(!fnt)
    {
        wmDeleteWindow(window);
        return -errno;
    }
    fntSetPointSize(fnt, 14, 96);

    pmPixMap_t *fileIcon = pmLoadPNG("/file.png");
    pmPixMap_t *clockIcon = pmLoadPNG("/clock_small.png");

    uiControl_t *rootControl = wmGetRootControl(window);
    //uiControlSetBackColor(rootControl, pmColorTransparent);
    //uiControlSetBackColor(rootControl, pmColorRed);
    uiControl_t *lbl = (uiControl_t *)uiLabelCreate(rootControl, 20, 1, pm->Contents.Width - 41, 24, "Date and time", NULL);
    uiControl_t *btn = (uiControl_t *)uiButtonCreate(rootControl, (pm->Contents.Width - 100) / 2, 30, 100, 80, "Do something", NULL);
    uiLineEdit_t *edit = uiLineEditCreate(rootControl, (pm->Contents.Width - 120) / 2, 120, 120, 30, "Trolololo", NULL);
    uiSlider_t *sld = uiSliderCreate(rootControl, (pm->Contents.Width - 120) / 2, 150, 120, 30, 1, 0, 100, 25);
    uiSlider_t *sld2 = uiSliderCreate(rootControl, 1, 40, 30, 130, 0, 0, 100, 25);

    uiControlSetBorderColor(rootControl, pmColorWhite);
    uiControlSetBorderStyle(rootControl, UI_BORDER_RAISED);

    uiControlSetTextColor(lbl, pmColorWhite);
    uiControlSetBackColor(lbl, pmColorDarkGray);
    uiControlSetTextIconSeparation(lbl, 4);
    uiControlSetIconPosition(lbl, UI_ICON_LEFT);
    uiControlSetIcon(lbl, clockIcon);

    uiControlSetIcon(btn, fileIcon);
    uiControlSetIconPosition(btn, UI_ICON_OVER);
    uiControlSetTextIconSeparation(btn, 2);
    uiControlSetOnMousePress(btn, btnClick);

    uiSliderSetOnValueChange(sld, sldChange);
    uiControlSetContext((uiControl_t *)sld, edit);

    uiControlRedraw(rootControl, 1);

    srand(time(NULL));
    ipcMessage_t msg;
    for(int i = 0;; ++i)
    {
        int quit = 0;

        int msgTimeout = 500;
        while(ipcGetMessage(&msg, msgTimeout) >= 0)
        {
            msgTimeout = 100;
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
                if(event->Type == WM_EVT_KEYBOARD)
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
        uiControlRedraw(lbl, 1);
    }

    printf("[usertest] Closing usertest\n");

    fntDelete(fnt);
    wmDeleteWindow(window);

    return 0;
}
