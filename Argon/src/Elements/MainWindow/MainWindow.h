#pragma once
#include "ElementBase/Element.h"

namespace MainWindowModule {
	class MainWindowClassManager : public WindowClassManager {
	public:
		static LRESULT CALLBACK MessageRouter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		void registerWindowClass();
		MainWindowClassManager();
	};

	extern MainWindowClassManager windowManager;

	class MainWindow : public Element {
		friend MainWindowClassManager;
	public:
		MainWindow(
			const wchar_t* title = L"Application",
			int w = 80, int h = 30
		);
	};
}

typedef MainWindowModule::MainWindow MainWindow;