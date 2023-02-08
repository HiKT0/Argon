#include "Input.h"

namespace InputModule {
	StyleSheet defaultStyles = {
		{"color", RGB(0, 0, 0)},
		{"background-color", RGB(255, 255, 255)}
	};
	Input::Input(int x, int y, int w, int h, const wchar_t* defaultText) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		this->text = defaultText;
		classManager = &windowManager;
		styles.defaults = &defaultStyles;
	}

	std::wstring Input::getText() {
		return winState.text;
	}

	InputClassManager windowManager;
}