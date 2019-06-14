#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <woot/uibutton.h>
#include <woot/uimenu.h>
#include <woot/vector.h>
#include <woot/wm.h>

typedef struct uiMenuItem uiMenuItem_t;

struct uiMenuItem
{
    char *Text;
    pmPixMap_t *Icon;
    uiMenu_t *SubMenu;
};

struct uiMenu
{
    vecVector_t *Items;
    wmWindow_t *Window;
};

uiMenu_t *uiMenuCreate()
{
    uiMenu_t *menu = (uiMenu_t *)calloc(1, sizeof(uiMenu_t));
    menu->Items = vecCreate(sizeof(uiMenuItem_t), 16, 16, 1024);
    if(!menu->Items)
    {
        uiMenuDelete(menu);
        return NULL;
    }
    return menu;
}

int uiMenuDelete(uiMenu_t *menu)
{
    if(!menu) return -EINVAL;
    if(menu->Items)
    {
        int itemCount = vecSize(menu->Items);
        for(int i = 0; i < itemCount; ++i)
        {
            uiMenuItem_t *item = (uiMenuItem_t *)vecGet(menu->Items, i);
            if(!item) continue;
            if(item->Text)
                free(item->Text);
        }
        vecDelete(menu->Items);
    }
    free(menu);
    return 0;
}

int uiMenuShow(uiMenu_t *menu, int x, int y)
{
    if(!menu) return -EINVAL;
    if(menu->Window)
    {
        wmSetWindowPos(menu->Window, x, y);
        return 0;
    }
    int itemCount = vecSize(menu->Items);
    if(itemCount <= 0) return -ENOENT;
    int itemHeight = 28;
    int width = 256;
    int height = itemCount * itemHeight;
    if(height < itemHeight) height = itemHeight;
    menu->Window = wmCreateWindow(x, y, width, height + 2, WM_CWF_POPUP);
    uiControl_t *root = wmGetRootControl(menu->Window);
    uiControlSetBorderStyle(root, UI_BORDER_RAISED);
    for(int i = 0; i < itemCount; ++i)
    {
        uiMenuItem_t *item = (uiMenuItem_t *)vecGet(menu->Items, i);
        uiControl_t *ic = (uiControl_t *)uiButtonCreate(root, 1, 1 + i * itemHeight, width - 2, itemHeight, item->Text);
        //uiControlSetBorderStyle(ic, UI_BORDER_NONE);
        uiControlSetIconPosition(ic, UI_LEFT);
        uiControlSetTextHAlign(ic, UI_HALIGN_LEFT);
        uiControlSetTextIconSeparation(ic, 4);
        if(item->Icon) uiControlSetIcon(ic, item->Icon);
    }
    uiControlRedraw(root, 1);
    return 0;
}

int uiMenuHide(uiMenu_t *menu)
{
    if(!menu) return -EINVAL;
    if(!menu->Window) return -EINVAL;
    wmDeleteWindow(menu->Window);
    menu->Window = NULL;
    return 0;
}

int uiMenuAddItem(uiMenu_t *menu, const char *text, pmPixMap_t *icon, uiMenu_t *subMenu)
{
    if(!menu || !menu->Items) return -EINVAL;
    uiMenuItem_t item;
    item.Text = strdup(text ? text : "");
    item.Icon = icon;
    item.SubMenu = subMenu;
    vecAppend(menu->Items, &item);
    return 0;
}
