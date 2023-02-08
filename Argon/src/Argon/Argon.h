#pragma once
#include "System/framework.h"
#include "Elements/MainWindow/MainWindow.h"

class Argon {
private:
	HACCEL accelTable;
	int nCmdShow;
public:
	MainWindow* window;
	Argon(const wchar_t* title, HINSTANCE applicationHandle, int nCmdShow);
	int run();
};