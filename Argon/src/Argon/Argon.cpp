#include "Argon.h"
#include "Shared/Globals.h"
#include <iostream>

Argon::Argon(const wchar_t* title, HINSTANCE applicationHandle, int nCmdShow) {
    this->nCmdShow = nCmdShow;

    Globals::applicationHandle = applicationHandle;
    window = new MainWindow(title, 480, 240);

    accelTable = LoadAccelerators(applicationHandle, L"AGRON");
}

int Argon::run() {
    ShowWindow(window->getWinHandle(), nCmdShow);
    UpdateWindow(window->getWinHandle());

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, accelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int)msg.wParam;
}