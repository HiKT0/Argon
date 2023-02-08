#include "MainWindow.h"
#include "Shared/Globals.h"

namespace MainWindowModule {
	MainWindow::MainWindow(const wchar_t* title, int w, int h) {
		this->text = title;
		this->x = CW_USEDEFAULT;
		this->y = 0;
		this->w = w;
		this->h = h;
		classManager = &windowManager;

		HWND winHandle = CreateWindow(
			classManager->winClass, text, classManager->style,
			x, y, w, h,
			NULL, (HMENU)0,
			Globals::applicationHandle, NULL
		);

		SetWindowLongPtrW(winHandle, GWLP_USERDATA, (LONG_PTR)this);
		windowHandle = winHandle;
	}

	MainWindowClassManager windowManager;
}