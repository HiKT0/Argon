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
		SelectObject(ps.hdc, Globals::font);

		const wchar_t* inputText = element->winState.text.c_str();
		int charactersToDraw = 0;
		int charactersWidth = 0;
		int inputWidth = rect.right - rect.left;
		int* widthBuf = new int[1];
		int currentCharIndex = element->winState.carriagePos;
		while (charactersWidth < inputWidth && currentCharIndex > 0) {
			wchar_t currentChar = inputText[currentCharIndex];
			GetCharWidth(hdc, currentChar, currentChar, widthBuf);
			charactersToDraw += 1;
			charactersWidth += *widthBuf;
			currentCharIndex -= 1;
		}
		delete[] widthBuf;

		const wchar_t* textToDraw = inputText + element->winState.text.length() - charactersToDraw;
		// волшебные манипул€ции со сдвигами
		SIZE stringSize = {};
		GetTextExtentPoint32W(
			hdc, textToDraw, charactersToDraw, &stringSize
		);

		SetBkMode(hdc, OPAQUE);
		SetTextColor(hdc, element->styles.get("color"));

		int rigthAnchor = stringSize.cx > (inputWidth - 10) ? (inputWidth - 10) : stringSize.cx + 2;

		RECT textRect = {
			.left = 2,
			.top = rect.top,
			.right = rigthAnchor,
			.bottom = rect.bottom
		};

		HBRUSH background = CreateSolidBrush(element->styles.get("background-color"));
		HBRUSH transparentBackground = (HBRUSH)GetStockObject(NULL_BRUSH);

		SelectObject(ps.hdc, background);
		RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, 10, 10);
		DrawText(
			hdc, textToDraw, charactersToDraw, &textRect,
			DT_SINGLELINE | DT_RIGHT | DT_VCENTER | DT_NOCLIP | DT_NOPREFIX
		);

		SelectObject(ps.hdc, transparentBackground);

		HPEN pen;
		pen = CreatePen(PS_SOLID, 5, element->styles.get("background-color"));
		SelectObject(hdc, pen);
		RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, 10, 10);
		DeleteObject(pen);

		pen = CreatePen(PS_SOLID, 1, element->styles.get("border-color"));
		SelectObject(hdc, pen);
		RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, 10, 10);
		DeleteObject(pen);

		int carriageX = textRect.right;
		int carriageOffsetY = (rect.bottom - rect.top - stringSize.cy) / 2;

		SetCaretPos(carriageX, carriageOffsetY);
		//MoveToEx(hdc, carriageX, carriageOffsetY, NULL);
		//LineTo(hdc, carriageX, stringSize.cy + carriageOffsetY);

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
			ShowCaret(hWnd);
			return 0;
		case WM_PAINT:
			draw(hWnd, element);
			return 0;
		case WM_CHAR: {
			element->winState.text += (wchar_t)wParam;
			element->winState.carriagePos += 1;
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		};
		case WM_SETFOCUS: {
			SIZE textSize{};
			HDC hdc = GetDC(hWnd);
			GetTextExtentPoint32(hdc, L"a", 1, &textSize);
			ReleaseDC(hWnd, hdc);
			CreateCaret(hWnd, 0, 1, textSize.cy);
			SetCaretPos(2, 0);
			return 0;
		}
		case WM_KILLFOCUS: {
			DestroyCaret();
			return 0;
		}

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
}