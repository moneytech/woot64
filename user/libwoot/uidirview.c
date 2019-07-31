#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <woot/uibutton.h>
#include <woot/uidirview.h>
#include <woot/uiscrollbar.h>
#include <woot/vector.h>
#include <woot/wm.h>

typedef uiButton_t fileTile_t;

struct uiDirView
{
    uiControl_t Control;
    char *Path;
    uiControl_t *FilesContainer;
    uiScrollbar_t *Scroll;
    vecVector_t *Infos;
    vecVector_t *Tiles;
    int TileWidth;
    int TileHeight;
    int TilesVisible;
    uiDirViewFileActivateHandler OnFileActivate;
};

static void filesContainerPaint(uiControl_t *sender)
{
    uiDrawDefaultBackground(sender);
    uiDrawChildren(sender);
    uiDrawDefaultBorder(sender);
}

static void fileTileActivate(uiControl_t *sender)
{
    fileTile_t *tile = (fileTile_t *)sender;
    if(!tile) return;
    uiDirView_t *view = (uiDirView_t *)(sender->Parent->Parent);
    if(!view) return;
    uiDirViewFileInfo_t *info = (uiDirViewFileInfo_t *)(sender->Context);
    if(!info) return;
    if(!view->OnFileActivate)
    {
        // TODO: add default handler
        return;
    }
    view->OnFileActivate(view, info);
}

static int readDir(uiDirView_t *view)
{
    DIR *dir = opendir(view->Path);
    if(!dir) return -errno;
    vecClear(view->Infos);
    struct dirent *de;
    uiDirViewFileInfo_t fi;
    int i;
    for(i = 0; (de = readdir(dir)); ++i)
    {
        snprintf(fi.Name, sizeof(fi.Name), "%s", de->d_name);
        stat(de->d_name, &fi.Stat);
        lstat(de->d_name, &fi.LStat);
        fi.OrigIdx = i;
        vecAppend(view->Infos, &fi);
    }
    closedir(dir);

    uiScrollbarSetMinPosition(view->Scroll, 0);
    uiScrollbarSetMaxPosition(view->Scroll, i + 1);
    uiScrollbarSetZoom(view->Scroll, view->TilesVisible);
    uiScrollbarSetPosition(view->Scroll, 0);
    uiControlRedraw((uiControl_t *)view->Scroll, UI_TRUE);

    return 0;
}

static void updateTiles(uiDirView_t *view)
{
    int fiCount = vecSize(view->Infos);
    int tiCount = vecSize(view->Tiles);

    pmColor_t defColor = wmGetColor(WM_COLOR_DEFAULT_TEXT);
    pmColor_t lnkColor = wmGetColor(WM_COLOR_LINK);

    int i = uiScrollbarGetPosition(view->Scroll), j = 0;
    for(; i < fiCount && j < tiCount; ++i, ++j)
    {
        uiDirViewFileInfo_t *fi = (uiDirViewFileInfo_t *)vecGet(view->Infos, i);
        if(!fi) break;
        fileTile_t *ti = *(fileTile_t **)vecGet(view->Tiles, j);
        if(!ti) break;
        int isLink = fi->Stat.st_ino != fi->LStat.st_ino;
        uiControlSetTextColor((uiControl_t *)ti, isLink ? lnkColor : defColor);
        uiControlSetText((uiControl_t *)ti, fi->Name);
        uiControlSetContext((uiControl_t *)ti, fi);
        uiControlSetTextHAlign((uiControl_t *)ti, UI_HALIGN_LEFT);
        uiControlSetIconPosition((uiControl_t *)ti, UI_LEFT);
        uiControlSetTextIconSeparation((uiControl_t *)ti, 4);
        uiControlSetIcon((uiControl_t *)ti,wmGetIcon(S_ISDIR(fi->Stat.st_mode) ? WM_ICON_DIRECTORY : (fi->Stat.st_mode & 0111 ? WM_ICON_PROGRAM : WM_ICON_FILE)));
        uiControlSetVisibility((uiControl_t *)ti, UI_VISIBLE);
    }

    for(; j < tiCount; ++j)
    {
        fileTile_t *ti = *(fileTile_t **)vecGet(view->Tiles, j);
        if(!ti) break;
        uiControlSetVisibility((uiControl_t *)ti, UI_HIDDEN);
        uiControlSetContext((uiControl_t *)ti, NULL);
    }
    uiControlRedraw(view->FilesContainer, UI_TRUE);
}

static void scrollbarChangePosition(uiScrollbar_t *scroll)
{
    uiDirView_t *view = (uiDirView_t *)uiControlGetParent((uiControl_t *)scroll);
    updateTiles(view);
}

uiDirView_t *uiDirViewCreate(uiControl_t *parent, int x, int y, int w, int h, const char *path)
{
    uiDirView_t *view = (uiDirView_t *)uiControlCreate(parent, sizeof(uiDirView_t), NULL, x, y, w, h, NULL);
    if(!view) return NULL;

    view->Path = strdup(path ? path : ".");
    if(!view->Path)
    {
        uiDirViewDelete(view);
        return NULL;
    }

    view->FilesContainer = uiControlCreate(&view->Control, 0, NULL, 0, 0, w - 17, h, NULL);
    if(!view->FilesContainer)
    {
        uiDirViewDelete(view);
        return NULL;
    }
    //uiControlSetBackColor((uiControl_t *)view->FilesContainer, pmColorWhite);
    uiControlSetBorderStyle((uiControl_t *)view->FilesContainer, UI_BORDER_SUNKEN);
    uiControlSetOnPaint((uiControl_t *)view->FilesContainer, filesContainerPaint);

    view->Scroll = uiScrollbarCreate(&view->Control, w - 16, 0, 16, h, UI_FALSE, 0, 0, 0, 0);
    if(!view->Scroll)
    {
        uiDirViewDelete(view);
        return NULL;
    }
    uiScrollbarSetOnChangePosition(view->Scroll, scrollbarChangePosition);

    view->Infos = vecCreate(sizeof(uiDirViewFileInfo_t), 32, 32, 0);
    if(!view->Infos)
    {
        uiDirViewDelete(view);
        return NULL;
    }

    view->Tiles = vecCreate(sizeof(fileTile_t *), 32, 32, 0);
    if(!view->Tiles)
    {
        uiDirViewDelete(view);
        return NULL;
    }

    view->TileWidth = w - 24;
    view->TileHeight = 36;
    view->TilesVisible = (h + view->TileHeight - 1) / view->TileHeight;
    for(int i = 0; i < view->TilesVisible; ++i)
    {
        fileTile_t *tile = uiButtonCreate(view->FilesContainer, 4, i * view->TileHeight + 1, view->TileWidth, view->TileHeight - 2, NULL);
        //uiControlSetVisibility((uiControl_t *)tile, UI_HIDDEN);
        uiControlSetOnActivate((uiControl_t *)tile, fileTileActivate);
        vecAppend(view->Tiles, &tile);
    }

    readDir(view);
    updateTiles(view);

    return view;
}

int uiDirViewDelete(uiDirView_t *view)
{
    if(!view) return -EINVAL;
    if(view->Tiles) vecDelete(view->Tiles);
    if(view->Infos) vecDelete(view->Infos);
    if(view->Path) free(view->Path);
    uiControlDelete(&view->Control);
    return 0;
}

int uiDirViewRefresh(uiDirView_t *view)
{
    if(!view) return -EINVAL;
    readDir(view);
    updateTiles(view);
    return 0;
}

int uiDirViewSetPath(uiDirView_t *view, const char *path)
{
    if(!view || !path || !path[0])
        return -EINVAL;
    if(view->Path) free(view->Path);
    view->Path = strdup(path);
    if(!view->Path)
        return -ENOMEM;
    return 0;
}

char *uiDirViewGetPath(uiDirView_t *view)
{
    if(!view)
    {
        errno = EINVAL;
        return NULL;
    };
    return view->Path;
}

int uiDirViewSetOnFileActivate(uiDirView_t *view, uiDirViewFileActivateHandler handler)
{
    if(!view) return -EINVAL;
    view->OnFileActivate = handler;
    return 0;
}

uiDirViewFileActivateHandler uiDirViewGetOnFileActivate(uiDirView_t *view)
{
    if(!view)
    {
        errno = EINVAL;
        return NULL;
    }
    return view->OnFileActivate;
}
