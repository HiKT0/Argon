#include "Button.h"
#include "Argon/Argon.h"
#include "Shared/Globals.h"

using namespace ButtonModule;

HFONT ButtonClassManager::font = 0;
int ButtonClassManager::event_n = 0;

LRESULT CALLBACK ButtonClassManager::MessageRouter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	Button* element = (Button*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	switch (message) {
	case WM_PAINT:
		draw(hWnd, element);
		return 0;
	case WM_LBUTTONDOWN:
		element->winState.isActive = true;
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
		return 0;
	case WM_LBUTTONUP:
	{
		element->winState.isActive = false;
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
		ClickEventDetails event{};

		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		event.windowHandle = hWnd;
		event.target = element;
		event.x = x;
		event.y = y;

		element->callListeners("click", event);

		return 0;
	}
	case WM_MOUSEMOVE:
		if (!element->winState.mouseHovering) {
			TRACKMOUSEEVENT tme{};
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwHoverTime = 1;
			tme.dwFlags = TME_HOVER | TME_LEAVE;
			tme.hwndTrack = hWnd;
			auto result = TrackMouseEvent(&tme);
			element->winState.mouseHovering = true;
		}
		return 0;
	case WM_MOUSEHOVER:
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
		return 0;
	case WM_SETFONT:
		font = (HFONT)wParam;
		if (lParam)
			draw(hWnd, element);
		return 0;
	case WM_MOUSELEAVE:
		element->winState.isActive = false;
		element->winState.mouseHovering = false;
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
};

void ButtonClassManager::draw(HWND hWnd, Button* e) {
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;

	GetClientRect(hWnd, &rect);

	hdc = BeginPaint(hWnd, &ps);

	COLORREF bgColor;

	if (e->winState.isActive)
		bgColor = e->styles.get("active-color");
	else {
		if (e->winState.mouseHovering)
			bgColor = e->styles.get("hover-color");
		else
			bgColor = e->styles.get("background-color");
	}

	HBRUSH background = CreateSolidBrush(bgColor);
	SelectObject(ps.hdc, background);
	SelectObject(ps.hdc, Globals::font);

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, e->styles.get("color"));
	

	RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, 10, 10);
	DrawText(hdc, e->text, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	EndPaint(hWnd, &ps);
	DeleteObject(background);
}

void ButtonClassManager::registerWindowClass() {
	WNDCLASS classStruct = {};
	classStruct.style = 0;
	classStruct.lpfnWndProc = MessageRouter;
	classStruct.hIcon = NULL;
	classStruct.hbrBackground = NULL;
	classStruct.hCursor = LoadCursor(nullptr, IDC_ARROW);
	classStruct.lpszMenuName = NULL;
	classStruct.lpszClassName = winClass;
	classStruct.hInstance = Globals::applicationHandle;

	RegisterClassW(&classStruct);
}

ButtonClassManager::ButtonClassManager() {
	style = WS_VISIBLE | WS_CHILD;
	winClass = L"ARG_BUTTON";
	registerWindowClass();
}
