#include "Button.h"

namespace ButtonModule {
	StyleSheet defaultStyles = {
		{"color", RGB(0, 0, 0)},
		{"background-color", RGB(240, 240, 240)},
		{"hover-color", RGB(220, 220, 220)},
		{"active-color", RGB(200, 200, 200)}
	};
	Button::Button(const wchar_t* text, int x, int y, int w, int h) {
		this->text = text;
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		classManager = &windowManager;
		styles.defaults = &defaultStyles;
	}

	ButtonClassManager windowManager;
}