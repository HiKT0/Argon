#pragma once
#include <map>
#include <string>
#include <initializer_list>
#include "System/framework.h"

class StyleSheet {
	std::map<std::string, COLORREF> properties;
public:
	StyleSheet* defaults = nullptr;
	StyleSheet* parent = nullptr;
	COLORREF get(std::string property);
	void set(std::string property, COLORREF value);
	StyleSheet(std::initializer_list<std::pair<std::string, COLORREF>> properties);
	StyleSheet();
};