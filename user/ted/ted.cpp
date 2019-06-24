#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <woot/ipc.h>
#include <woot/ui.h>
#include <woot/uitextedit.h>
#include <woot/wm.h>

static wmWindow_t *window = nullptr;
static uiControl_t *rootControl = nullptr;

void *operator new(unsigned long size)
{
    return calloc(1, size);
}

void *operator new[](unsigned long size)
{
    return calloc(1, size);
}

void operator delete(void *ptr)
{
    free(ptr);
}

void operator delete[](void *ptr)
{
    free(ptr);
}

extern "C" int main(int argc, char *argv[])
{
    int res = 0;
    if((res = wmInitialize(0)) < 0)
    {
        fprintf(stderr, "[ted] wmInitialize() failed\n");
        return res;
    }

    if(!(window = wmCreateWindow(WM_CW_USEDEFAULT, WM_CW_USEDEFAULT, 700, 500, WM_CWF_DEFAULT)))
    {
        fprintf(stderr, "[ted] wmCreateWindow() failed\n");
        wmCleanup();
        return -errno;
    }

    char *filename = argc > 1 ? argv[1] : nullptr;
    {
        char buf[64];
        snprintf(buf, sizeof(buf), "Text editor - %s", filename ? filename : "untitled");
        wmSetWindowTitle(window, buf);
    }

    rootControl = wmGetRootControl(window);
    rcRectangle_t rootRect = uiControlGetRect(rootControl);
    uiControlSetBorderStyle(rootControl, UI_BORDER_RAISED);

    uiTextEdit_t *textEdit = uiTextEditCreate(rootControl, 2, 2, rootRect.Width - 4, rootRect.Height - 4);
    uiControlSetFont((uiControl_t *)textEdit, wmGetFont(WM_FONT_MONO));
    if(filename)
    {
        if(FILE *f = fopen(filename, "rb"))
        {
            char *lineBuf = nullptr;
            size_t allocated = 0;
            while((res = getline(&lineBuf, &allocated, f)) > 0)
            {
                if(lineBuf[res - 1] == '\n')
                    lineBuf[res - 1] = 0;
                uiTextEditAppendLine(textEdit, lineBuf);
            }
            if(lineBuf) free(lineBuf);
            fclose(f);
        }
    }

    uiTextEditSetEditable(textEdit, UI_TRUE);
    uiControlSetFocus((uiControl_t *)textEdit);
    uiControlRedraw(rootControl, 1);

    ipcMessage_t msg;
    for(;;)
    {
        bool quit = false;
        while(ipcGetMessage(&msg, -1) >= 0)
        {
            ipcProcessMessage(&msg);
            if(msg.Number == MSG_QUIT)
            {
                quit = true;
                break;
            }
            else if(msg.Number == MSG_WM_EVENT)
            {
                wmEvent_t *event = (wmEvent_t *)msg.Data;
                if(event->WindowId == wmGetWindowId(window))
                    wmProcessEvent(window, event);
                switch(event->Type)
                {
                case WM_EVT_CLOSE:
                    quit = true;
                    break;
                default:
                    break;
                }
                if(quit) break;
            }
        }
        if(quit) break;
    }

    wmDeleteWindow(window);
    wmCleanup();
    return 0;
}

