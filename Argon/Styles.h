#pragma once
#include "framework.h"

class StyleSheet {
public:
	COLORREF textColor = RGB(0, 0, 0);
	HBRUSH backgroundColor = CreateSolidBrush(RGB(240, 240, 240));
	HBRUSH hoverColor = CreateSolidBrush(RGB(220, 220, 220));
	HBRUSH activeColor = CreateSolidBrush(RGB(200, 200, 200));
};