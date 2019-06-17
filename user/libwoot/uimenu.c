#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <woot/uibutton.h>
#include <woot/uimenu.h>
#include <woot/vector.h>
#include <woot/vkeys.h>
#include <woot/wm.h>

typedef struct uiMenuItem uiMenuItem_t;

struct uiMenuItem
{
    char *Text;
    pmPixMap_t *Icon;
    uiMenu_t *SubMenu;
    uiEventHandler Handler;
};

struct uiMenu
{
    vecVector_t *Items;
    wmWindow_t *Window;
};

static void rootKeyPress(uiControl_t *control, wmEvent_t *event)
{
    if(!control || !event)
        return;

    uiMenu_t *menu = (uiMenu_t *)uiControlGetContext(control);
    if(event->Keyboard.Key == VK_ESCAPE)
        uiMenuHide(menu);
}

static void windowMouseLeave(wmWindow_t *sender, wmEvent_t *event)
{
    uiControl_t *root = wmGetRootControl(sender);
    if(!root) return;
    uiMenu_t *menu = (uiMenu_t *)uiControlGetContext(root);
    if(!menu) return;
    uiMenuHide(menu);
}

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
        wmShowWindow(menu->Window);
        wmActivateWindow(menu->Window);
        return 0;
    }
    int itemCount = vecSize(menu->Items);
    if(itemCount <= 0) return -ENOENT;
    int itemHeight = 28;
    int width = 256;
    int height = itemCount * itemHeight;
    if(height < itemHeight) height = itemHeight;

    // create popup window
    menu->Window = wmCreateWindow(x, y, width, height + 2, WM_CWF_POPUP);
    wmSetOnMouseLeave(menu->Window, windowMouseLeave);

    // get root control
    uiControl_t *root = wmGetRootControl(menu->Window);
    uiControlSetContext(root, menu);
    uiControlSetBorderStyle(root, UI_BORDER_RAISED);
    uiControlSetOnKeyPress(root, rootKeyPress);

    // create item controls
    for(int i = 0; i < itemCount; ++i)
    {
        uiMenuItem_t *item = (uiMenuItem_t *)vecGet(menu->Items, i);
        uiControl_t *ic = (uiControl_t *)uiButtonCreate(root, 1, 1 + i * itemHeight, width - 2, itemHeight, item->Text);
        uiControlSetCanHaveFocus(ic, UI_FALSE);
        uiControlSetBorderStyle(ic, UI_BORDER_NONE);
        uiControlSetIconPosition(ic, UI_LEFT);
        uiControlSetTextHAlign(ic, UI_HALIGN_LEFT);
        uiControlSetTextIconSeparation(ic, 4);
        uiControlSetOnActivate(ic, item->Handler);
        if(item->Icon) uiControlSetIcon(ic, item->Icon);
    }

    uiControlRedraw(root, 1);
    return 0;
}

int uiMenuHide(uiMenu_t *menu)
{
    if(!menu) return -EINVAL;
    if(!menu->Window) return -EINVAL;
    //wmDeleteWindow(menu->Window);
    //menu->Window = NULL;
    wmHideWindow(menu->Window);
    return 0;
}

int uiMenuAddItem(uiMenu_t *menu, const char *text, pmPixMap_t *icon, uiMenu_t *subMenu, uiEventHandler handler)
{
    if(!menu || !menu->Items) return -EINVAL;
    uiMenuItem_t item;
    item.Text = strdup(text ? text : "");
    item.Icon = icon;
    item.SubMenu = subMenu;
    item.Handler = handler;
    vecAppend(menu->Items, &item);
    return 0;
}

int uiMenuProcessEvent(uiMenu_t *menu, wmEvent_t *event)
{
    if(!menu || !event) return -EINVAL;
    if(!menu->Window || event->Handled || wmGetWindowId(menu->Window) != event->WindowId)
        return 0;

    return wmProcessEvent(menu->Window, event);
    /*uiControl_t *rootControl = wmGetRootControl(menu->Window);
    if(!rootControl)
        return -EINVAL;

    return uiControlProcessEvent(rootControl, event);*/
}
