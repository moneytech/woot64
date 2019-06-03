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
#include <woot/uilineedit.h>
#include <woot/uitoolbar.h>
#include <woot/wm.h>

static wmWindow_t *window;
static uiControl_t *rootControl;
static rcRectangle_t rootSize;
static uiControl_t *files;
static pmPixMap_t *file;
static pmPixMap_t *directory;
static uiLineEdit_t *leAddress;

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

static void fileActivate(uiControl_t *sender);

static void loadDir(const char *dirname)
{
    if(chdir(dirname) < 0)
        return;

    char cwd[64];
    getcwd(cwd, sizeof(cwd));
    wmSetWindowTitle(window, cwd);
    uiControlSetText((uiControl_t *)leAddress, cwd);

    DIR *dir = opendir(".");
    if(dir)
    {
        struct stat st;
        dirent *de;
        for(int i = 0; (de = readdir(dir)); ++i)
        {
            if(!strcmp(de->d_name, "."))
            {
                --i;
                continue;
            }
            uiButton_t *btn = uiButtonCreate(files, 4, 4 + i * 42, 256, 40, de->d_name);
            uiControlSetOnActivate((uiControl_t *)btn, fileActivate);
            uiControlSetTextHAlign((uiControl_t *)btn, UI_HALIGN_LEFT);
            uiControlSetIconPosition((uiControl_t *)btn, UI_LEFT);
            uiControlSetTextIconSeparation((uiControl_t *)btn, 8);
            if(!stat(de->d_name, &st))
                uiControlSetIcon((uiControl_t *)btn, S_ISDIR(st.st_mode) ? directory : file);
        }
        closedir(dir);
    }
}

static int processPath(const char *path);

static void fileActivate(uiControl_t *sender)
{
    char *text = strdup(uiControlGetText(sender));
    processPath(text);
    free(text);
}

static void upActivate(uiControl_t *sender)
{
    processPath("..");
}

static void addressAccept(uiLineEdit_t *edit)
{
    processPath(uiControlGetText((uiControl_t *)edit));
}

static int processPath(const char *path)
{
    struct stat st;
    if(!stat(path, &st))
    {
        if(S_ISDIR(st.st_mode))
        {
            if(files) uiControlDelete(files);
            files = uiControlCreate(rootControl, 0, nullptr, 1, 34, rootSize.Width - 2, rootSize.Height - 35, nullptr);
            loadDir(path);
            uiControlRedraw(rootControl, 1);
        }
        else if(st.st_mode & 0111)
            processCreate(path);
    }
    return 0;
}

extern "C" int main(int argc, char *argv[])
{
    setbuf(stdout, nullptr);

    if(argc > 1)
        chdir(argv[1]);

    file = pmLoadPNG("/data/common/icons/file.png");
    directory = pmLoadPNG("/data/common/icons/directory.png");

    wmInitialize(WM_INITIALIZE_NONE);

    window = wmCreateWindow(WM_CW_USEDEFAULT, WM_CW_USEDEFAULT, 500, 600, WM_CWF_DEFAULT);
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

    files = uiControlCreate(rootControl, 0, nullptr, 1, 34, rootSize.Width - 2, rootSize.Height - 35, nullptr);

    loadDir(cwd);

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
