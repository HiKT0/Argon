#pragma once
#include "framework.h"

class WindowClassManager {
public:
	static DWORD style;
	static LPCWSTR winClass;
	virtual WNDCLASS getWinStruct(HINSTANCE applicationHandle) = 0;
	static LRESULT CALLBACK MessageRouter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void draw(HWND hWnd);
};