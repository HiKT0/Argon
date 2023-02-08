#pragma once
#include "ElementBase/Element.h"
#include <iostream>
#include "Shared/Styles.h"

namespace ButtonModule {
	class Button;

	class ButtonClassManager : public WindowClassManager {
	public:
		static int event_n;
		static LRESULT CALLBACK MessageRouter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static void draw(HWND hWnd, Button* e);
		static HFONT font;
		void registerWindowClass();
		ButtonClassManager();
	};

	extern ButtonClassManager windowManager;
	extern StyleSheet defaultStyles;

	struct ButtonWindowState {
		bool isActive = false;
		bool mouseHovering = false;
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
}

typedef ButtonModule::Button Button;