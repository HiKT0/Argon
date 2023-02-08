#include "System/framework.h"
#include "Argon/Argon.h"
#include "Shared/Globals.h"
#include "Elements/Button/Button.h"
#include "Elements/Input/Input.h"

int initDebugConsole(HWND winHandle) {
#ifdef _DEBUG
    AllocConsole();
    if (freopen("conin$", "r", stdin) == nullptr
        || freopen("conout$", "w", stdout) == nullptr
        || freopen("conout$", "w", stderr) == nullptr) {
        MessageBox(winHandle, L"Не удалось инициализировать консоль", L"Ошибка", MB_OK);
        return FALSE;
    }
    printf("Debugging Window:\n");
    return TRUE;
#else
    return TRUE;
#endif
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    Argon* argon = new Argon(L"Application", hInstance, nCmdShow);

    if (!initDebugConsole(argon->window->getWinHandle()))
        return FALSE;

    Globals::font = CreateFontA(
        16, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, 65001,
        OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, "Comic Sans"
    );

    Button* btn1 = new Button(L"Кнопка 1");
    Input* input = new Input(10, 40);

    btn1->addListener("click", [&input](Event& e) {
        MessageBox(e.windowHandle, input->getText().c_str(), L"Э", MB_OK);
    });

    argon->window->add(btn1);
    argon->window->add(input);

    
    return argon->run();
}