#pragma once

#include "resource.h"
#include "framework.h"
#include <vector>
#include <string>
#include <map>

#include "WindowClassManager.h"
#include "Styles.h"

#define MAX_LOADSTRING 100

class Element;
class Argon;

struct Event {
	HWND windowHandle;
	Element* target;
};

struct ClickEvent : Event {
	int x, y;
	int button;
	bool ctrlHeld, shiftHeld, LMBHeld;
};

class Argon {
private:
	const int ELEMENT_ID_OFFSET = 200;
	int windowsCounter;
	std::vector<Element*> elements;
	HWND addWindow(LPCWSTR winClass, LPCWSTR text, DWORD style, int x, int y, int w, int h);
	const wchar_t* szTitle;
	const wchar_t* szWindowClass;

	static LRESULT CALLBACK MessageRouter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	ATOM RegisterMainClass(HINSTANCE hInstance);

	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	HINSTANCE applicationHandle;
	HACCEL accelTable;
	int nCmdShow;
public:
	HWND mainWindowHandle;
	Element* add(Element* e);
	Argon(const wchar_t* title, HINSTANCE applicationHandle, int nCmdShow);
	int run();
};

typedef void (*EventCallback)(Event& details);

class Element {
	friend Argon;
protected:
	void callListeners(const char* event, Event& details);
	HWND windowHandle = 0;
	int id = -1;
	StyleSheet styles;
	static WindowClassManager* classManager;
	std::map<std::string, std::vector<EventCallback>> callbacks;
public:
	LPCWSTR text;
	int x = 0, y = 0, w = 0, h = 0;
	void addListener(const char* event, EventCallback callback);
	void removeListener(const char* event, EventCallback callback);
};