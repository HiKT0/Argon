#include "Shared/Globals.h"
#include "MainWindow.h"
#include <iostream>

using namespace MainWindowModule;

LRESULT CALLBACK MainWindowClassManager::MessageRouter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	
	switch (message) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_LBUTTONDOWN:
		SetFocus(hWnd);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void MainWindowClassManager::registerWindowClass() {
	WNDCLASS wcex = {};
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MessageRouter;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = Globals::applicationHandle;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = L"Argon";
	wcex.lpszClassName = winClass;

	RegisterClassW(&wcex);
}
MainWindowClassManager::MainWindowClassManager() {
	style = WS_OVERLAPPEDWINDOW;
	winClass = L"ARG_MAIN_WINDOW";
	registerWindowClass();
}
