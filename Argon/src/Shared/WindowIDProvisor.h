#pragma once
class WindowIDProvisor {
private:
	static int counter;
public:
	// WindowIDProvisor();
	int getNext();
};

