#include "Button.h"

int ButtonClassManager::event_n = 0;

LRESULT CALLBACK ButtonClassManager::MessageRouter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	Button* element = (Button*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch (message) {
	case WM_PAINT:
		draw(hWnd, element);
		return 0;
	case WM_LBUTTONDOWN:
		element->winState.isActive = true;
		UpdateWindow(hWnd);
		return 0;
	case WM_LBUTTONUP:
	{
		element->winState.isActive = false;
		UpdateWindow(hWnd);
		ClickEvent event{};

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
		UpdateWindow(hWnd);
		return 0;
	case WM_MOUSELEAVE:
		element->winState.isActive = false;
		element->winState.mouseHovering = false;
		UpdateWindow(hWnd);
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

	if (e->winState.isActive)
		SelectObject(ps.hdc, e->styles.activeColor);
	else {
		if (e->winState.mouseHovering)
			SelectObject(ps.hdc, e->styles.hoverColor);
		else
			SelectObject(ps.hdc, e->styles.backgroundColor);
	}
	
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, e->styles.textColor);

	RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, 10, 10);
	DrawText(hdc, e->text, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	EndPaint(hWnd, &ps);
}

WNDCLASS ButtonClassManager::getWinStruct(HINSTANCE applicationHandle) {
	WNDCLASS winClass = {};
	winClass.style = 139;
	winClass.lpfnWndProc = MessageRouter;
	winClass.cbWndExtra = 8;
	winClass.hIcon = NULL;
	winClass.hbrBackground = NULL;
	winClass.lpszMenuName = NULL;
	winClass.lpszClassName = L"ARG_BUTTON";
	winClass.hInstance = applicationHandle;

	return winClass;
}

Button::Button(const wchar_t* text, int x, int y, int w, int h)
{
	this->text = text;
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;

	if (classManager == nullptr) {
		classManager = new ButtonClassManager();
	}
}