#include "framework.h"
#include "Argon.h"
#include "Button.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
#ifdef _DEBUG
    AllocConsole();
    freopen("conin$", "r", stdin);
    freopen("conout$", "w", stdout);
    freopen("conout$", "w", stderr);
    printf("Debugging Window:\n");
#endif

    Argon* argon = new Argon(L"Application", hInstance, nCmdShow);


    Button* btn1 = new Button(L"Кнопка 1");
    Button* btn2 = new Button(L"хочу и жму", 100, 10, 90, 30);

    btn1->addListener("click", [](Event& e) {
        MessageBox(e.windowHandle, L"че жмешь", L"Э", MB_OK);
    });

    btn2->addListener("click", [](Event& e) {
        MessageBox(e.windowHandle, L"ну лан жми", L"э.", MB_OK);
    });

    argon->add(btn1);
    argon->add(btn2);

    return argon->run();
}