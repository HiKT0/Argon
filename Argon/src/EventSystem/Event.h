#pragma once
#include "System/framework.h"
#include "ElementBase/Element.h"
#include <functional>

class Element;

struct Event {
	HWND windowHandle;
	Element* target;
};

struct ClickEventDetails : Event {
	int x, y;
	int button;
	bool ctrlHeld, shiftHeld, LMBHeld;
};

struct CreateEventDetails : Event {

};

typedef std::function<void (Event& event)> EventCallback;