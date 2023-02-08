#pragma once
#include "System/framework.h"

class WindowClassManager {
public:
	DWORD style = 0;
	LPCWSTR winClass = 0;
	virtual void registerWindowClass() = 0;
};