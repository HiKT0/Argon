#include "Argon.h"
#include <iostream>

Element* Argon::add(Element* e) {
    WNDCLASS wcex;
    bool winClassFound = GetClassInfo(applicationHandle, e->classManager->winClass, &wcex);

    if (!winClassFound) {
        WNDCLASS classStruct = e->classManager->getWinStruct(applicationHandle);
        RegisterClass(&classStruct);
    }

    HWND winHandle = addWindow(
        e->classManager->winClass, e->text, e->classManager->style,
        e->x, e->y, e->w, e->h
    );
    if (winHandle == NULL) {
        DWORD error = GetLastError();
        LPWSTR buffer = nullptr;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, error, NULL, buffer, 0, NULL);
        std::wcerr << buffer << std::endl;
    }
    SetWindowLongPtr(winHandle, GWLP_USERDATA, (LONG_PTR)e);
    elements.push_back(e);

    e->id = elements.size() - 1;
    e->windowHandle = winHandle;

    return e;
}

HWND Argon::addWindow(LPCWSTR winClass, LPCWSTR text, DWORD style, int x, int y, int w, int h) {
    WNDCLASSEX wcex;
    bool winClassFound = GetClassInfoEx(applicationHandle, winClass, &wcex);
    if (!winClassFound) {
        MessageBox(mainWindowHandle, L"Ќе удаетс€ зарегистрировать класс", L"ќшибка", MB_OK);
    }
    
    HWND win = CreateWindow(
        winClass, text, style,
        x, y, w, h,
        mainWindowHandle, (HMENU)windowsCounter,
        applicationHandle, NULL
    );
    windowsCounter++;
    return win;
}

LRESULT Argon::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

        // слушатель закрыти€ окна
        if (wmId == IDM_EXIT) {
            DestroyWindow(hWnd);
            return 0;
        }

        // передать управление обработчику по умолчанию, если окно не в нашей зоне ответственности
        if (!(wmId >= ELEMENT_ID_OFFSET && wmId < ELEMENT_ID_OFFSET + elements.size()))
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: ƒобавьте сюда любой код прорисовки, использующий HDC...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT Argon::MessageRouter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Argon* instance;
    if (message == WM_CREATE)
        instance = (Argon*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
    else
        instance = (Argon*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    return instance->WndProc(hWnd, message, wParam, lParam);
}

ATOM Argon::RegisterMainClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = MessageRouter;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ARGON));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_ARGON);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL Argon::InitInstance(HINSTANCE hInstance, int nCmdShow) {
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 480, 240, nullptr, nullptr, hInstance, nullptr);

    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);

    if (!hWnd)
    {
        return FALSE;
    }

    mainWindowHandle = hWnd;
    return TRUE;
}

Argon::Argon(const wchar_t* title, HINSTANCE applicationHandle, int nCmdShow) {
    this->applicationHandle = applicationHandle;
    this->nCmdShow = nCmdShow;

    szWindowClass = L"ARGON_APPLICATION";
    szTitle = title;

    RegisterMainClass(applicationHandle);

    if (!InitInstance(applicationHandle, nCmdShow))
    {
        // TODO: сделать обработку случа€, когда не удалось инициализировать
        return;
    }

    accelTable = LoadAccelerators(applicationHandle, MAKEINTRESOURCE(IDC_ARGON));

    windowsCounter = ELEMENT_ID_OFFSET;
}

int Argon::run() {
    ShowWindow(mainWindowHandle, nCmdShow);
    UpdateWindow(mainWindowHandle);

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

WindowClassManager* Element::classManager = nullptr;

void Element::addListener(const char* event, EventCallback callback) {
    callbacks[event].push_back(callback);
}

void Element::removeListener(const char* event, EventCallback callback) {
    if (!callbacks.contains(event))
        return;
    std::vector<EventCallback>& searchArray = callbacks[event];
    for (int i = 0; i < searchArray.size(); i++) {
        if (searchArray[i] == callback)
            searchArray.erase(searchArray.begin() + i);
    }
}

void Element::callListeners(const char* event, Event& details) {
    if (callbacks.contains(event)) {
        for (EventCallback function : callbacks[event]) {
            function(details);
        }
    }
}