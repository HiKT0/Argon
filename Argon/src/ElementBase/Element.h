#pragma once
#include <vector>
#include <map>
#include <string>
#include "EventSystem/Event.h"
#include "Shared/WindowClassManager.h"
#include "Shared/windowIDProvisor.h"
#include "Shared/Styles.h"

class Element {
protected:
	static WindowIDProvisor winIdProvisor;
	void callListeners(const char* event, Event& details);
	HWND windowHandle = 0;
	WindowClassManager* classManager = 0;
	std::map<std::string, std::vector<EventCallback>> callbacks;
	HWND addWindow(LPCWSTR winClass, LPCWSTR text, DWORD style, int x, int y, int w, int h);
	LPCWSTR text;
	void showLastError();
public:
	std::vector<Element*> children;
	Element* parent;
	StyleSheet styles;

	bool add(Element* e);
	HWND getWinHandle();
	int x = 0, y = 0, w = 0, h = 0;
	void addListener(const char* event, EventCallback callback);
	void removeListener(const char* event, EventCallback callback);
};