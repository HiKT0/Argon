#include "Element.h"
#include "Shared/Globals.h"
#include "System/framework.h"
#include <iostream>

bool Element::add(Element* e) {
    HWND winHandle = addWindow(
        e->classManager->winClass, e->text, e->classManager->style,
        e->x, e->y, e->w, e->h
    );
    
    if (winHandle == NULL) {
        showLastError();
        return false;
    }

    SetWindowLongPtrW(winHandle, GWLP_USERDATA, (LONG_PTR)e);
    SendMessage(winHandle, WM_SETFONT, (WPARAM)Globals::font, TRUE);
    e->windowHandle = winHandle;

    children.push_back(e);

    CreateEventDetails event{};
    event.target = e;
    event.windowHandle = winHandle;

    e->callListeners("create", event);

    return true;
}

void Element::addListener(const char* event, EventCallback callback) {
    callbacks[event].push_back(callback);
}

void Element::removeListener(const char* event, EventCallback callback) {
    // TODO: доделать
    //if (!callbacks.contains(event))
    //    return;
    //std::vector<EventCallback>& searchArray = callbacks[event];
    //for (int i = 0; i < searchArray.size(); i++) {
    //    if (searchArray[i] == callback)
    //        searchArray.erase(searchArray.begin() + i);
    //}
}

void Element::callListeners(const char* event, Event& details) {
    if (callbacks.contains(event)) {
        for (EventCallback function : callbacks[event]) {
            function(details);
        }
    }
}

HWND Element::addWindow(LPCWSTR winClass, LPCWSTR text, DWORD style, int x, int y, int w, int h) {
    WNDCLASSEX wcex;
    bool winClassFound = GetClassInfoEx(Globals::applicationHandle, winClass, &wcex);
    if (!winClassFound) {
        MessageBox(
            windowHandle,
            L"Не удается добавить окно",
            L"Ошибка", MB_OK || MB_ICONWARNING
        );
    }
    HINSTANCE appHandle = Globals::applicationHandle;
    HMENU menu = (HMENU)winIdProvisor.getNext();

    HWND win = CreateWindow(
        winClass, text, style,
        x, y, w, h,
        windowHandle, menu,
        appHandle, NULL
    );
    return win;
}

void Element::showLastError()
{
    DWORD error = GetLastError();
    LPWSTR buffer = new wchar_t[1024];
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, error,
        NULL, buffer,
        1024, NULL
    );
    std::wcerr << buffer << std::endl;
}

HWND Element::getWinHandle() {
    return windowHandle;
}

WindowIDProvisor Element::winIdProvisor;