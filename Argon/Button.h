#pragma once
#include "framework.h"
#include "Argon.h"
#include <iostream>

class Button;

struct ButtonWindowState {
	bool isActive = false;
	bool mouseHovering = false;
};

class ButtonClassManager : public WindowClassManager {
	friend Button;
protected:
	static int event_n;
	//void ButtonClassManager::onClick(EventType event, Event& details);
	static LRESULT CALLBACK MessageRouter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void draw(HWND hWnd, Button* e);
	
	WNDCLASS getWinStruct(HINSTANCE applicationHandle);
public:
	DWORD style = BS_FLAT | WS_VISIBLE | WS_CHILD;
	LPCWSTR winClass = L"ARG_BUTTON";
};

class Button : public Element {
	friend ButtonClassManager;
	StyleSheet styles;
	ButtonWindowState winState;
public:
	Button(
		const wchar_t* text = L"Button", 
		int x = 10, int y = 10, 
		int w = 80, int h = 30
	);
};