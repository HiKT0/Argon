#include "Input.h"
#include "Shared/Globals.h"

#include <iostream>

namespace InputModule {
	void InputClassManager::registerWindowClass() {
		WNDCLASS classStruct = {};
		classStruct.style = 0;
		classStruct.lpfnWndProc = MessageRouter;
		classStruct.hCursor = LoadCursor(nullptr, IDC_IBEAM);
		classStruct.lpszClassName = winClass;
		classStruct.hInstance = Globals::applicationHandle;
		RegisterClassW(&classStruct);
	}

	void InputClassManager::draw(HWND hWnd, Input* element) {
		PAINTSTRUCT ps;
		HDC hdc;
		RECT rect;

		GetClientRect(hWnd, &rect);

		hdc = BeginPaint(hWnd, &ps);
		HBRUSH background = CreateSolidBrush(element->styles.get("background-color"));

		SelectObject(ps.hdc, background);
		SelectObject(ps.hdc, Globals::font);

		// волшебные манипул€ции со сдвигами
		SIZE stringSize = {};
		GetTextExtentPoint32(
			hdc, element->winState.text.c_str(), 
			element->winState.text.length(), &stringSize
		);

		int xOverflow = stringSize.cx - (rect.right - rect.left);
		int textOffset = xOverflow > 0 ? -xOverflow - 6: 0;

		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, element->styles.get("color"));

		RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, 10, 10);
		RECT textRect = {
			.left = rect.left + textOffset, 
			.top = rect.top,
			.right = rect.right,
			.bottom = rect.bottom
		};
		DrawText(hdc, element->winState.text.c_str(), -1, &textRect, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

		//MoveToEx(hdc, 0, 0, NULL);
		//LineTo(hdc, 0, 10);

		EndPaint(hWnd, &ps);
		DeleteObject(background);
	}

	InputClassManager::InputClassManager() {
		style = WS_VISIBLE | WS_CHILD;
		winClass = L"ARG_INPUT";
		registerWindowClass();
	}

	LRESULT InputClassManager::MessageRouter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		Input* element = (Input*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		switch (message) {
		case WM_LBUTTONDOWN:
			SetFocus(hWnd);
			return 0;
		case WM_PAINT:
			draw(hWnd, element);
			return 0;
		case WM_CHAR: {
			element->winState.text += (wchar_t)wParam;
			element->winState.carriageX += 1;
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}
			
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
}