#pragma once

#include <stdio.h>
#include <sys/stat.h>
#include <woot/ui.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct uiDirViewFileInfo
{
    char Name[FILENAME_MAX];
    struct stat Stat;
    int OrigIdx;
} uiDirViewFileInfo_t;

typedef struct uiDirView uiDirView_t;
typedef void (*uiDirViewFileActivateHandler)(uiDirView_t *sender, uiDirViewFileInfo_t *info);

uiDirView_t *uiDirViewCreate(uiControl_t *parent, int x, int y, int w, int h, const char *path);
int uiDirViewDelete(uiDirView_t *view);
int uiDirViewRefresh(uiDirView_t *view);
int uiDirViewSetPath(uiDirView_t *view, const char *path);
char *uiDirViewGetPath(uiDirView_t *view);
int uiDirViewSetOnFileActivate(uiDirView_t *view, uiDirViewFileActivateHandler handler);
uiDirViewFileActivateHandler uiDirViewGetOnFileActivate(uiDirView_t *view);

#ifdef __cplusplus
}
#endif // __cplusplus
