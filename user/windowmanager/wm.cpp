#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <woot/thread.h>
#include <woot/timer.h>
#include <woot/uibutton.h>
#include <woot/uitoolbar.h>
#include <woot/wm.h>

#include "wm.hpp"
#include "window.hpp"

#define MOUSE_ACCELERATION 0

extern "C" int main(int argc, char *argv[])
{
    int screenWidth = 640;
    int screenHeight = 480;
    int screenDepth = 32;

    if(argc > 1) screenWidth = atoi(argv[1]);
    if(argc > 2) screenHeight = atoi(argv[2]);
    if(argc > 3) screenDepth = atoi(argv[3]);

    setbuf(stdout, nullptr);

    printf("[windowmanager] Started window manager (pid: %d)\n", getpid());

    int defaultDisplayId = vidGetDefaultDisplayId();
    if(defaultDisplayId < 0)
    {
        printf("[windowmanager] Couldn't get default display id\n");
        return -errno;
    }
    int display = vidOpenDisplay(defaultDisplayId);
    if(display < 0)
    {
        printf("[windowmanager] Couldn't open default display\n");
        return -errno;
    }

    printf("[windowmanager] Trying %dx%dx%d video mode\n", screenWidth, screenHeight, screenDepth);
    if(vidSetMode2(display, screenWidth, screenHeight, screenDepth, -1, -1) < 0)
    {
        printf("[windowmanager] Couldn't set video mode\n");
        return -errno;
    }

    int currMode = vidGetCurrentMode(display);
    if(currMode < 0)
    {
        printf("[windowmanager] Couldn't get current video mode number\n");
        return -errno;
    }

    vidModeInfo_t mi;
    if(vidGetModeInfo(display, currMode, &mi) < 0)
    {
        printf("[windowmanager] Couldn't get current video mode information\n");
        return -errno;
    }

    void *pixels = vidMapPixels(display, nullptr);
    if(pixels == reinterpret_cast<void *>(-1))
    {
        printf("[windowmanager] Couldn't get frame buffer data\n");
        return -errno;
    }

    pmPixelFormat_t fbFormat = pmFormatFromModeInfo(&mi);

    pmPixMap_t *fbPixMap = pmFromMemory(mi.Width, mi.Height, mi.Pitch, &fbFormat, pixels, 0);
    pmPixMap_t *bbPixMap = pmFromPixMap(fbPixMap, &fbFormat); // create back buffer

    ipcSendMessage(0, MSG_ACQUIRE_KEYBOARD, MSG_FLAG_NONE, nullptr, 0);
    ipcSendMessage(0, MSG_ACQUIRE_MOUSE, MSG_FLAG_NONE, nullptr, 0);

    threadDaemonize();

    ipcMessage_t msg;
    WindowManager *wm = new WindowManager(fbPixMap, bbPixMap);
    for(int i = 0;; ++i)
    {
        rcRectangle_t dirtyRect = rcRectangleEmpty;
        ipcGetMessage(&msg, -1);
        bool quit = false;
        do
        {
            ipcProcessMessage(&msg);
            if(wm->ProcessMessage(&msg, &dirtyRect))
            {
                quit = true;
                break;
            }
        } while(!quit && ipcGetMessage(&msg, 0) >= 0);
        if(quit) break;

        if(!rcIsEmptyP(&dirtyRect))
            wm->UpdateRect(&dirtyRect);
    }

    printf("[windowmanager] Closing window manager\n");
    ipcSendMessage(0, MSG_RELEASE_KEYBOARD, MSG_FLAG_NONE, nullptr, 0);
    ipcSendMessage(0, MSG_RELEASE_MOUSE, MSG_FLAG_NONE, nullptr, 0);

    if(wm) delete wm;
    if(bbPixMap) pmDelete(bbPixMap);
    if(fbPixMap) pmDelete(fbPixMap);
    vidCloseDisplay(display);

    return 0;
}

void WindowManager::taskButtonActivate(uiControl_t *control)
{
    uiControlClearFocus(control);

    Window *wnd = reinterpret_cast<Window *>(control->Context);
    if(!wnd) return;

    WindowManager *wm = wnd->WM;
    if(!wm) return;

    wm->bringToFront(wnd);
    wm->topWindow = wnd;
    wm->setActiveWindow(wnd);

    if(!wnd->Visible)
        wm->RestoreWindow(wnd);
}

Window *WindowManager::getWindowById(int id)
{
    for(Window *wnd : *windows)
    {
        if(wnd->GetId() == id)
            return wnd;
    }
    return nullptr;
}

void WindowManager::moveWindow(rcRectangle_t *dirtyRect, Window *window, int x, int y)
{
    if(window->Visible)
        *dirtyRect = rcAdd(*dirtyRect, window->GetDecoratedRect());
    window->SetPosition(x, y);
    if(window->Visible)
    {
        *dirtyRect = rcAdd(*dirtyRect, window->GetDecoratedRect());
        handleMouseEnterLeave();
    }
}

int WindowManager::getWindowIdx(Window *wnd)
{
    size_t wndCnt = windows->size();
    for(size_t i = 0; i < wndCnt; ++i)
    {
        if(windows->at(i) == wnd)
            return static_cast<int>(i);
    }
    return -1;
}

void WindowManager::bringToFront(Window *wnd)
{
    int wndIdx = getWindowIdx(wnd);
    int topWndIdx = getWindowIdx(findFrontWindow());
    if(wndIdx >= 0 && topWndIdx >= 0 && topWndIdx > wndIdx)
    {
        windows->erase(windows->begin() + wndIdx);
        windows->insert(windows->begin() + topWndIdx, 1, wnd);
        rcRectangle_t wndRect = wnd->GetDecoratedRect();
        UpdateRect(&wndRect);
    }
    handleMouseEnterLeave();
}

void WindowManager::sendToBack(Window *wnd)
{
    int wndIdx = getWindowIdx(wnd);
    int backWndIdx = getWindowIdx(findBackWindow());
    if(wndIdx >= 0 && backWndIdx >= 0 && backWndIdx < wndIdx)
    {
        windows->erase(windows->begin() + wndIdx);
        windows->insert(windows->begin() + backWndIdx, 1, wnd);
        rcRectangle_t wndRect = wnd->GetDecoratedRect();
        UpdateRect(&wndRect);
    }
    handleMouseEnterLeave();
}

Window *WindowManager::findFrontWindow()
{
    Window *prevWnd = nullptr;
    size_t wndCnt = windows->size();
    for(size_t i = 0; i < wndCnt; ++i)
    {
        Window *wnd = windows->at(i);
        if(!wnd->Visible) continue;
        if(wnd == taskWnd || wnd == mouseWnd) break;
        prevWnd = wnd;
    }
    return prevWnd;
}

Window *WindowManager::findBackWindow()
{
    size_t wndCnt = windows->size();
    for(size_t i = 0; i < wndCnt; ++i)
    {
        Window *wnd = windows->at(i);
        if(wnd == desktopWnd || !wnd->Visible) continue;
        if(wnd == taskWnd || wnd == mouseWnd) break;
        return wnd;
    }
    return nullptr;
}

Window *WindowManager::findByCoords(int x, int y)
{
    Window *prevWnd = nullptr;
    size_t wndCnt = windows->size();
    for(size_t i = 0; i < wndCnt; ++i)
    {
        Window *wnd = windows->at(i);
        rcRectangle_t rect = wnd->GetDecoratedRect();
        if(!wnd->Visible || !rcContainsPointP(&rect, x, y))
            continue;
        if(wnd == taskWnd || wnd == mouseWnd) break;
        prevWnd = wnd;
    }
    return prevWnd;
}

void WindowManager::handleMouseEnterLeave()
{
    wmEvent_t event;
    Window *wnd = findByCoords(mouseX, mouseY);
    if(!wnd) return;
    int wndId = wnd->GetId();
    if(oldMouseWndId != wndId)
    {   // send mouse enter/leave messages
        memset(&event, 0, sizeof(event));
        event.Type = WM_EVT_MOUSE_LEAVE;
        event.WindowId = oldMouseWndId;
        Window *oldWnd = getWindowById(oldMouseWndId);
        if(oldWnd) ipcSendMessage(oldWnd->GetOwner(), MSG_WM_EVENT, MSG_FLAG_NONE, &event, sizeof(event));
        event.Type = WM_EVT_MOUSE_ENTER;
        event.WindowId = wndId;
        ipcSendMessage(wnd->GetOwner(), MSG_WM_EVENT, MSG_FLAG_NONE, &event, sizeof(event));
    }
    oldMouseWndId = wndId;
}

void WindowManager::hideWindow(Window *wnd)
{
    wnd->Visible = false;
    if(wnd == activeWindow)
    {   // deactivate window if needed
        Window *front = findFrontWindow();
        setActiveWindow(front);
    }
    handleMouseEnterLeave();
}

void WindowManager::showWindow(Window *wnd)
{
    wnd->Visible = true;
    handleMouseEnterLeave();
}

void WindowManager::setActiveWindow(Window *window)
{
    if(activeWindow != window)
    {
        if(activeWindow)
        {
            caretTick(0);
            rcRectangle_t rc = activeWindow->SetActive(false);
            activeWindow->UpdateWindowGraphics(&rc);
            if(activeWindow->TaskButton)
            {
                uiButtonSetSelected(activeWindow->TaskButton, UI_FALSE);
                UpdateRect(&taskRect);
            }
            UpdateRect(&rc);
        }
        activeWindow = window;
        if(activeWindow)
        {
            caretTick(1);
            rcRectangle_t rc = activeWindow->SetActive(true);
            activeWindow->UpdateWindowGraphics(&rc);
            if(activeWindow->TaskButton)
            {
                uiButtonSetSelected(activeWindow->TaskButton, UI_TRUE);
                UpdateRect(&taskRect);
            }
            UpdateRect(&rc);
        }
    }
}

void WindowManager::wmEventMouse(rcRectangle_t *rect, int wndId, int mouseX, int mouseY, wmEvent_t *event, inpMouseEvent_t *mouseEv)
{
    memset(event, 0, sizeof(*event));
    event->Type = WM_EVT_MOUSE;
    event->WindowId = wndId;
    event->Handled = 0;
    event->Mouse.Coords[0] = mouseX - rect->X;
    event->Mouse.Coords[1] = mouseY - rect->Y;
    for(int i = 0; i < WM_EVT_MOUSE_AXES && i < INP_MAX_MOUSE_AXES; ++i)
        event->Mouse.Delta[i] = mouseEv->Delta[i];
    event->Mouse.ButtonsPressed = mouseEv->ButtonsPressed;
    event->Mouse.ButtonsHeld = mouseEv->ButtonsHeld;
    event->Mouse.ButtonsReleased = mouseEv->ButtonsReleased;
}

void WindowManager::caretTick(int visible)
{
    if(!activeWindow)
        return;
    wmEvent_t event;
    memset(&event, 0, sizeof(event));
    event.Type = WM_EVT_CARET_TICK;
    event.WindowId = activeWindow->GetId();
    event.Handled = 0;
    if(visible < 0)
    {
        event.CaretTick.Visible = caretVisible != 0 ? 1 : 0;
        caretVisible = !caretVisible;
    }
    else
    {
        caretVisible = visible;
        event.CaretTick.Visible = caretVisible != 0 ? 1 : 0;
    }
    ipcSendMessage(activeWindow->GetOwner(), MSG_WM_EVENT, MSG_FLAG_NONE, &event, sizeof(event));
}

void WindowManager::windowsRemove(Window *wnd)
{
    size_t size = windows->size();
    for(size_t i = 0; i < size; ++i)
    {
        if(wnd == windows->at(i))
        {
            windows->erase(windows->begin() + static_cast<int>(i));
            return;
        }
    }
}

WindowManager::WindowManager(pmPixMap_t *fbPixMap, pmPixMap_t *bbPixMap) :
    windows(new Windows),
    fbPixMap(fbPixMap),
    bbPixMap(bbPixMap)
{
    int currentPId = getpid();
    timerInitialize();
    wmInitialize(WM_INITIALIZE_WM);
    deskRect = bbPixMap->Contents;

    caretTimer = timerCreate(750, 0);
    timerStart(caretTimer);

    // create desktop
    desktopWnd = new Window(this, currentPId, 0, 0, deskRect.Width, deskRect.Height, WM_CWF_NONE, bbPixMap, nullptr);
    windows->insert(windows->begin(), desktopWnd);
    pmPixMap_t *logo = pmLoadPNG("/logo.png");
    if(logo && desktopWnd)
    {
        pmPixMap_t *pm = desktopWnd->GetPixMap();
        pmClear(pm, pmColorFromRGB(24, 64, 96));
        pmAlphaBlit(pm, logo, 0, 0,
                    (deskRect.Width - logo->Contents.Width) / 2,
                    (deskRect.Height - logo->Contents.Height) / 2,
                    logo->Contents.Width, logo->Contents.Height);
        pmDelete(logo);
    }

    // create taskbar
    taskWnd = new Window(this, currentPId, 0, deskRect.Height - 28, deskRect.Width, 28, WM_CWF_NONE, bbPixMap, nullptr);
    windows->push_back(taskWnd);
    taskRect = taskWnd->GetRect();
    taskRoot = uiControlCreate(nullptr, 0, taskWnd->GetPixMap(), 0, 0, taskRect.Width, taskRect.Height, nullptr);
    uiControlSetBorderStyle(taskRoot, UI_BORDER_RAISED);
    taskBar = uiToolbarCreate(taskRoot, 1, 1, taskRect.Width - 2, taskRect.Height - 2, UI_HORIZONTAL);
    uiToolbarSetChildSpacing(taskBar, 1);
    uiControlRedraw(taskRoot, UI_FALSE);

    // create mouse cursor window
    pmPixMap_t *cursor = pmLoadCUR("/normal.cur", 0, &cursorHotX, &cursorHotY);
    if(cursor)
    {
        mouseWnd = new Window(this, currentPId, mouseX - cursorHotX, mouseY - cursorHotY,
                              cursor->Contents.Width, cursor->Contents.Height,
                              WM_CWF_USEALPHA, bbPixMap, &cursor->Format);
        pmBlit(mouseWnd->GetPixMap(), cursor, 0, 0, 0, 0, -1, -1);
        pmDelete(cursor);
        windows->push_back(mouseWnd);
    }

    UpdateRect(&bbPixMap->Contents);
}

int WindowManager::ProcessMessage(ipcMessage_t *msg, rcRectangle_t *dirtyRect)
{
    if(msg->Number == MSG_QUIT)
    {
        threadSleep(THREAD_SELF, 500);
        return 1;
    }
    else if(msg->Number == MSG_TIMER)
    {
        timerMsg_t *tmsg = reinterpret_cast<timerMsg_t *>(msg->Data);
        if(tmsg->Id == caretTimer)
            caretTick(-1);
    }
    else if(msg->Number == MSG_WM_EVENT)
    {
        wmEvent_t *event = reinterpret_cast<wmEvent_t *>(msg->Data);
        if(event->WindowId == taskWnd->GetId())
        {
            uiControlProcessEvent((uiControl_t *)taskBar, event);
            *dirtyRect = rcAddP(dirtyRect, &taskRect);
        }
    }
    else if(msg->Number == MSG_KEYBOARD_EVENT)
    {
        static bool altTabPhase = true;
        static bool changeAltTabPhase = false;
        inpKeyboardEvent_t *kbdEv = (inpKeyboardEvent_t *)msg->Data;
        inpProcessKeyboardEvent(kbdEv, &modifiers);
        int chr = kbdEv->Flags & INP_KBD_EVENT_FLAG_RELEASE ? 0 : inpTranslateKey(kbdEv->Key, modifiers);
        //printf("[usertest] key: %d %s\n", kbdEv->Key, kbdEv->Flags & INP_KBD_EVENT_FLAG_RELEASE ? "released" : "pressed");
        if(modifiers & INP_MOD_ALT && kbdEv->Key == VK_TAB && kbdEv->Flags & INP_KBD_EVENT_FLAG_RELEASE)
        {   // alt+tab switch
            if((modifiers & INP_MOD_SHIFT) != altTabPhase)
            {
                Window *backWnd = findBackWindow();
                if(backWnd) bringToFront(backWnd);
            }
            else
            {
                Window *frontWnd = findFrontWindow();
                if(frontWnd) sendToBack(frontWnd);
            }
            topWindow = findFrontWindow();
            setActiveWindow(topWindow);
            changeAltTabPhase = true;
        }
        else if(changeAltTabPhase && (kbdEv->Key == VK_LMENU || kbdEv->Key == VK_RMENU) && kbdEv->Flags & INP_KBD_EVENT_FLAG_RELEASE)
        {
            altTabPhase = !altTabPhase;
            changeAltTabPhase = false;
        }
        else if(activeWindow)
        {
            wmEvent_t event;
            event.Type = WM_EVT_KEYBOARD;
            event.WindowId = activeWindow->GetId();
            event.Handled = 0;
            event.Keyboard.Key = kbdEv->Key;
            event.Keyboard.Flags = kbdEv->Flags;
            event.Keyboard.Character = chr;
            ipcSendMessage(activeWindow->GetOwner(), MSG_WM_EVENT, MSG_FLAG_NONE, &event, sizeof(event));
        }
    }
    else if(msg->Number == MSG_MOUSE_EVENT)
    {
        inpMouseEvent_t *mouseEv = (inpMouseEvent_t *)msg->Data;

        // mouse acceleration
#if MOUSE_ACCELERATION
        float mouseSpeed = mouseEv->Delta[0] * mouseEv->Delta[0] + mouseEv->Delta[1] * mouseEv->Delta[1];
        if(mouseSpeed > 16)
        {
            mouseEv->Delta[0] += mouseEv->Delta[0] >> 1;
            mouseEv->Delta[1] += mouseEv->Delta[1] >> 1;
        }
#endif // MOUSE_ACCELL

        mouseX += mouseEv->Delta[0];
        mouseY += mouseEv->Delta[1];

        if(mouseX < 0) mouseX = 0;
        else if(mouseX >= deskRect.Width)
            mouseX = deskRect.Width - 1;
        if(mouseY < 0) mouseY = 0;
        else if(mouseY >= deskRect.Height)
            mouseY = deskRect.Height - 1;

        if(mouseWnd)
            moveWindow(dirtyRect, mouseWnd, mouseX - cursorHotX, mouseY - cursorHotY);

        for(int i = windows->size() - 1; i >= 0; --i)
        {
            Window *wnd = windows->at(i);
            if(!wnd->Visible) continue;
            int wndId = wnd->GetId();

            if(wnd == mouseWnd || wnd == desktopWnd)
                continue;

            if(mouseEv->ButtonsReleased & 1)
                dragWindow = nullptr;

            bool doBreak = false;

            // handle window activation and deactivation
            rcRectangle_t rect = wnd->GetDecoratedRect();
            if(rcContainsPointP(&rect, mouseX, mouseY))
            {
                doBreak = true;
                if(mouseEv->ButtonsPressed & 1)
                {
                    if(topWindow != wnd) // bring window to the top
                    {
                        bringToFront(wnd);
                        topWindow = wnd;
                    }
                    setActiveWindow(wnd);
                }
            }

            // handle titlebar buttons
            rcRectangle_t decoRect = rect;
            rect = wnd->GetTitleRect();
            if(rcContainsPointP(&rect, mouseX, mouseY))
            {
                uiControl_t *titleBar = wnd->GetTitleControl();
                if(titleBar)
                {
                    doBreak = true;
                    wmEvent_t event;
                    wmEventMouse(&rect, wndId, mouseX, mouseY, &event, mouseEv);
                    uiControlProcessEvent(titleBar, &event);
                    pmPixMap_t *pm = uiControlGetPixMap(titleBar);
                    rcRectangle_t dr = pmGetAndClearDirtyRectangle(pm);
                    if(!rcIsEmptyP(&dr))
                    {
                        dr.X += rect.X;
                        dr.Y += rect.Y;
                        *dirtyRect = rcAddP(dirtyRect, &dr);
                    }
                }
            }

            // handle window dragging
            rect = wnd->GetDragRect();
            if(rcContainsPointP(&rect, mouseX, mouseY))
            {
                if(mouseEv->ButtonsPressed & 1)
                {
                    dragWindow = wnd;
                    dragDeltaX = mouseX - decoRect.X;
                    dragDeltaY = mouseY - decoRect.Y;
                }
            }
            if(dragWindow && (mouseEv->Delta[0] || mouseEv->Delta[1]))
                moveWindow(dirtyRect, dragWindow, mouseX - dragDeltaX, mouseY - dragDeltaY);

            // handle client area events
            rect = wnd->GetRect();
            if(rcContainsPointP(&rect, mouseX, mouseY))
            {
                doBreak = true;
                handleMouseEnterLeave();
                wmEvent_t event;
                wmEventMouse(&rect, wndId, mouseX, mouseY, &event, mouseEv);
                ipcSendMessage(wnd->GetOwner(), MSG_WM_EVENT, MSG_FLAG_NONE, &event, sizeof(event));
            }

            if(doBreak)
                break;
        }
    }
    else if(msg->Number == MSG_RPC_REQUEST)
    {
        char *req = (char *)msg->Data;
        char *args = (char *)(req + strlen(req) + 1);
        if(!strcmp(req, "wmCreateWindow"))
        {
            int x = *(int *)(args);
            int y = *(int *)(args + 4);
            unsigned w = *(unsigned *)(args + 8);
            unsigned h = *(unsigned *)(args + 12);
            unsigned flags = *(unsigned *)(args + 16);

            if(x == WM_CW_CENTER) x = (deskRect.Width - w) / 2;
            else if(x == WM_CW_USEDEFAULT)
            {
                if((defX + w) > deskRect.Width)
                    defX = 32;
                x = defX;
                defX += 32;
            }
            if(y == WM_CW_CENTER) y = (deskRect.Height - h) / 2;
            else if(y == WM_CW_USEDEFAULT)
            {
                if((defY + h) > deskRect.Height)
                    defY = 32;
                y = defY;
                defY += 32;
            }

            //printf("[windowmanager] wmCreateWindow(%d, %d, %d, %d, %#.8x)\n", x, y, w, h, flags);
            Window *wnd = new Window(this, msg->Source, x, y, w, h, flags, bbPixMap, nullptr);
            *dirtyRect = rcAdd(*dirtyRect, wnd->GetDecoratedRect());

            // add new window on top
            Window *frontWnd = findFrontWindow();
            int frontIdx = getWindowIdx(frontWnd);
            windows->insert(windows->begin() + frontIdx + 1, wnd);

            wmCreateWindowResp response;

            response.id = wnd->GetId();
            response.pixelFormat = wnd->GetPixelFormat();
            snprintf(response.shMemName, MSG_RPC_RESP_PAYLOAD_SIZE - ((uintptr_t)(response.Data) - (uintptr_t)&response), "%s", wnd->GetShMemName());

            // create taskbar button if needed
            if(flags & WM_CWF_APPWINDOW)
            {
                wnd->TaskButton = uiButtonCreate((uiControl_t *)taskBar, 0, 0, 192, 24, nullptr);
                if(wnd->TaskButton)
                {
                    uiControlSetOnActivate((uiControl_t *)wnd->TaskButton, taskButtonActivate);
                    uiControlSetContext((uiControl_t *)wnd->TaskButton, wnd);
                    uiControlSetTextHAlign((uiControl_t *)wnd->TaskButton, UI_HALIGN_LEFT);
                    uiControlRecalcRects((uiControl_t *)taskBar);
                    uiControlRedraw((uiControl_t *)taskRoot, UI_FALSE);
                    *dirtyRect = rcAddP(dirtyRect, &taskRect);
                }
            }

            topWindow = wnd;
            setActiveWindow(wnd);
            handleMouseEnterLeave();
            rpcIPCReturn(msg->Source, msg->ID, &response, sizeof(response));
        }
        else if(!strcmp(req, "wmDeleteWindow"))
        {
            int id = *reinterpret_cast<int *>(args);
            Window *wnd = getWindowById(id);
            if(wnd)
            {
                rcRectangle_t rect = wnd->GetDecoratedRect();
                *dirtyRect = rcAddP(dirtyRect, &rect);                
                windowsRemove(wnd);
                if(dragWindow == wnd)
                    dragWindow = nullptr;
                topWindow = findFrontWindow();
                setActiveWindow(topWindow);
                handleMouseEnterLeave();

                // delete taskbar button
                if(wnd->TaskButton)
                {
                    uiButtonDelete(wnd->TaskButton);
                    uiControlRecalcRects(reinterpret_cast<uiControl_t *>(taskBar));
                    uiControlRedraw(reinterpret_cast<uiControl_t *>(taskRoot), UI_FALSE);
                    *dirtyRect = rcAddP(dirtyRect, &taskRect);
                }

                delete wnd;
            }
            rpcIPCReturn(msg->Source, msg->ID, nullptr, 0);
        }
        else if(!strcmp(req, "wmRedrawWindow"))
        {
            int window = *reinterpret_cast<int *>(args);
            Window *wnd = getWindowById(window);
            rcRectangle rect = wnd ? wnd->GetDecoratedRect() : rcRectangleEmpty;
            *dirtyRect = rcAddP(dirtyRect, &rect);
            rpcIPCReturn(msg->Source, msg->ID, nullptr, 0);
        }
        else if(!strcmp(req, "wmRedrawRect"))
        {
            wmRedrawRectArgs *rra = reinterpret_cast<wmRedrawRectArgs *>(args);
            //printf("rra %d %d %d %d\n", rra->rect.X, rra->rect.Y, rra->rect.Width, rra->rect.Height);
            Window *wnd = getWindowById(rra->windowId);
            rcRectangle_t wndRect = wnd->GetRect();
            rra->rect.X += wndRect.X;
            rra->rect.Y += wndRect.Y;
            rra->rect = rcIntersectP(&wndRect, &rra->rect);
            *dirtyRect = rcAddP(dirtyRect, &rra->rect);
            rpcIPCReturn(msg->Source, msg->ID, nullptr, 0);
        }
        else if(!strcmp(req, "wmSetWindowPos"))
        {
            wmSetWindowPosArgs *swpa = reinterpret_cast<wmSetWindowPosArgs *>(args);
            Window *wnd = getWindowById(swpa->windowId);
            if(wnd) moveWindow(dirtyRect, wnd, swpa->x, swpa->y);
            rpcIPCReturn(msg->Source, msg->ID, nullptr, 0);
        }
        else if(!strcmp(req, "wmSetWindowTitle"))
        {
            wmSetWindowTitleArgs *swta = reinterpret_cast<wmSetWindowTitleArgs *>(args);
            rpcIPCReturn(msg->Source, msg->ID, nullptr, 0);

            Window *wnd = getWindowById(swta->windowId);
            if(wnd)
            {
                wnd->SetTitle(swta->title);
                *dirtyRect = rcAdd(*dirtyRect, wnd->GetDecoratedRect());
            }

            // modify taskbar button
            if(wnd->TaskButton)
            {
                uiControlSetText(reinterpret_cast<uiControl_t *>(wnd->TaskButton), swta->title);
                *dirtyRect = rcAddP(dirtyRect, &taskRect);
            }
        }
        else if(!strcmp(req, "wmGetMousePos"))
        {
            wmGetMousePosResp resp;
            resp.X = mouseX;
            resp.Y = mouseY;
            rpcIPCReturn(msg->Source, msg->ID, &resp, sizeof(resp));
        }
        else if(!strcmp(req, "wmShowWindow"))
        {
            int id = *reinterpret_cast<int *>(args);
            Window *wnd = getWindowById(id);
            if(wnd)
            {
                showWindow(wnd);
                *dirtyRect = rcAdd(*dirtyRect, wnd->GetDecoratedRect());
            }
            rpcIPCReturn(msg->Source, msg->ID, nullptr, 0);
        }
        else if(!strcmp(req, "wmHideWindow"))
        {
            int id = *reinterpret_cast<int *>(args);
            Window *wnd = getWindowById(id);
            if(wnd && wnd->Visible)
            {
                hideWindow(wnd);
                *dirtyRect = rcAdd(*dirtyRect, wnd->GetDecoratedRect());
            }
            rpcIPCReturn(msg->Source, msg->ID, nullptr, 0);
        }
        else if(!strcmp(req, "wmActivateWindow"))
        {
            int id = *reinterpret_cast<int *>(args);
            Window *wnd = getWindowById(id);
            if(wnd && wnd->Visible)
                setActiveWindow(wnd);
            rpcIPCReturn(msg->Source, msg->ID, nullptr, 0);
        }
        else printf("[windowmanager] Unknown RPC request '%s' from process %d\n", req, msg->Source);
    }
    else if(msg->Number == MSG_RPC_FIND_SERVER && !strcmp("windowmanager", reinterpret_cast<const char *>(msg->Data)))
        rpcIPCFindServerRespond(msg->Source, msg->ID);
    return 0;
}

void WindowManager::UpdateRect(rcRectangle_t *rect)
{
    for(Window *wnd : *windows)
    {
        if(!wnd->Visible)
            continue;
        wnd->UpdateWindowGraphics(rect);
    }
    pmBlit(fbPixMap, bbPixMap, rect->X, rect->Y, rect->X, rect->Y, rect->Width, rect->Height);
    //pmRectangleRect(fbPixMap, rect, pmColorFromRGB(rand(), rand(), rand()));
}

void WindowManager::GetMousePos(int *x, int *y)
{
    if(x) *x = mouseX;
    if(y) *y = mouseY;
}

void WindowManager::MinimizeWindow(Window *window)
{
    if(!window->Visible)
        return;
    window->WM->hideWindow(window);
    rcRectangle_t rect = window->GetDecoratedRect();
    window->WM->UpdateRect(&rect);
}

void WindowManager::RestoreWindow(Window *window)
{
    if(window->Visible)
        return;
    window->WM->showWindow(window);
    rcRectangle_t rect = window->GetDecoratedRect();
    window->WM->UpdateRect(&rect);
}

WindowManager::~WindowManager()
{
    // cleanup timers
    if(caretTimer > 0)
        timerDelete(caretTimer);
    timerCleanup();

    if(taskWnd)
    {
        windowsRemove(taskWnd);
        delete taskWnd;
    }
    if(mouseWnd)
    {
        windowsRemove(mouseWnd);
        delete mouseWnd;
    }
    if(desktopWnd)
    {
        windowsRemove(desktopWnd);
        delete desktopWnd;
    }
    wmCleanup();
}
