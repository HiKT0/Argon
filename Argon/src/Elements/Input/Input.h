#pragma once
#include <string>
#include "ElementBase/Element.h"
namespace InputModule {
	class Input;
	class InputClassManager : public WindowClassManager {
	public:
		void registerWindowClass();
		static LRESULT CALLBACK MessageRouter(HWND hWnd, UINT message, WPARAM wparam, LPARAM lParam);
		static void draw(HWND hWnd, Input* element);
		InputClassManager();
	};

	struct InputWindowState {
		std::wstring text;
		int carriagePos = 0;
	};

	extern StyleSheet defaultStyles;

	class Input : public Element {
		friend InputClassManager;
		InputWindowState winState;
	public:
		Input(int x = 10, int y = 10, int w = 80, int h = 20, const wchar_t* defaultText = L"");
		std::wstring getText();
	};

	extern InputClassManager windowManager;
}

typedef InputModule::Input Input;