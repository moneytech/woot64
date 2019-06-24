#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <woot/ipc.h>
#include <woot/process.h>
#include <woot/ui.h>
#include <woot/uibutton.h>
#include <woot/uidirview.h>
#include <woot/uilineedit.h>
#include <woot/uitoolbar.h>
#include <woot/wm.h>

static wmWindow_t *window;
static uiControl_t *rootControl;
static rcRectangle_t rootSize;
static uiLineEdit_t *leAddress;
static uiDirView_t *view;

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

static void processPath(const char *path, struct stat *st = nullptr)
{
    bool freest = false;
    if(!st)
    {
        freest = true;
        st = new struct stat;
        stat(path, st);
    }
    int cwdSize = 4096;
    char *cwd = new char[cwdSize];
    if(cwd && chdir(path) >= 0)
    {
        getcwd(cwd, cwdSize);
        wmSetWindowTitle(window, cwd);
        uiControlSetText((uiControl_t *)leAddress, cwd);
        wmSetWindowTitle(window, cwd);
        uiDirViewSetPath(view, cwd);
        uiDirViewRefresh(view);
    }
    if(cwd) delete[] cwd;
    if(freest) delete st;
}

static void fileActivate(uiDirView_t *view, uiDirViewFileInfo_t *info)
{
    processPath(info->Name, &info->Stat);
}

static void upActivate(uiControl_t *sender)
{
    processPath("..");
}

static void addressAccept(uiLineEdit_t *edit)
{
    processPath(uiControlGetText((uiControl_t *)edit));
}

extern "C" int main(int argc, char *argv[])
{
    setbuf(stdout, nullptr);

    if(argc > 1)
        chdir(argv[1]);

    wmInitialize(WM_INITIALIZE_NONE);

    window = wmCreateWindow(WM_CW_USEDEFAULT, WM_CW_USEDEFAULT, 500, 450, WM_CWF_DEFAULT);
    if(!window)
    {
        fprintf(stderr, "[filemanager] Couldn't create main window\n");
        return -errno;
    }

    char cwd[256];
    getcwd(cwd, sizeof(cwd));

    rootControl = wmGetRootControl(window);
    wmSetWindowTitle(window, "File manager");
    uiControlSetBorderStyle(rootControl, UI_BORDER_RAISED);
    rootSize = uiControlGetSize(rootControl);

    fntFont_t *uiSymFont = wmGetFont(WM_FONT_UI_SYMBOLS);

    uiToolbar_t *toolbar = uiToolbarCreate(rootControl, 1, 1, rootSize.Width - 2, 32, UI_HORIZONTAL);
    uiToolbarSetChildSpacing(toolbar, 1);

    uiButton_t *btnBack = uiButtonCreate((uiControl_t *)toolbar, 0, 0, 28, 28, "<");
    uiControlSetFont((uiControl_t *)btnBack, uiSymFont);

    uiButton_t *btnFwd = uiButtonCreate((uiControl_t *)toolbar, 0, 0, 28, 28, ">");
    uiControlSetFont((uiControl_t *)btnFwd, uiSymFont);

    uiButton_t *btnUp = uiButtonCreate((uiControl_t *)toolbar, 0, 0, 28, 28, "^");
    uiControlSetFont((uiControl_t *)btnUp, uiSymFont);
    uiControlSetOnActivate((uiControl_t *)btnUp, upActivate);

    int leWidth = uiToolbarGetFreeSpace(toolbar);
    leAddress = uiLineEditCreate((uiControl_t *)toolbar, 0, 0, leWidth, 28, cwd);
    uiLineEditSetOnAcceptInput(leAddress, addressAccept);

    uiControlRecalcRects((uiControl_t *)toolbar);

    view = uiDirViewCreate(rootControl, 2, 34, rootSize.Width - 4, rootSize.Height - 36, cwd);
    uiDirViewSetOnFileActivate(view, fileActivate);

    uiControlRedraw(rootControl, 1);
    wmRedrawWindow(window);

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
                if(event->Type == WM_EVT_CLOSE)
                {
                    quit = true;
                    break;
                }
            }
        }
        if(quit) break;
    }

    wmDeleteWindow(window);
    wmCleanup();
    return 0;
}
