#include <errno.h>
#include <stdio.h>
#include <woot/ipc.h>
#include <woot/ui.h>
#include <woot/wm.h>

extern "C" int main(int argc, char *argv[])
{
    wmInitialize(WM_INITIALIZE_NONE);

    wmWindow_t *window = wmCreateWindow(WM_CW_USEDEFAULT, WM_CW_USEDEFAULT, 500, 400, WM_CWF_DEFAULT);
    if(!window)
    {
        fprintf(stderr, "[filemanager] Couldn't create main window\n");
        return -errno;
    }

    uiControl_t *rootControl = wmGetRootControl(window);
    wmSetWindowTitle(window, "File manager");
    uiControlSetBorderStyle(rootControl, UI_BORDER_RAISED);
    uiControlRedraw(rootControl, 1);
    wmRedrawWindow(window);

    ipcMessage_t msg;
    for(;;)
    {
        bool quit = false;
        for(;;)
        {
            int res = ipcGetMessage(&msg, -1);
            if(res < 0 || msg.Number == MSG_QUIT)
            {
                quit = true;
                break;
            }
        }
        if(quit) break;
    }

    wmCleanup();
    return 0;
}
