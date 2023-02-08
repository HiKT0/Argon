#include "Styles.h"

COLORREF StyleSheet::get(std::string property)
{
    if (properties.contains(property))
        return properties[property];
    if (parent != nullptr)
        return parent->get(property);
    if (defaults != nullptr && defaults->properties.contains(property))
        return defaults->get(property);
    return RGB(0, 0, 0);
}

void StyleSheet::set(std::string property, COLORREF value) {
    properties[property] = value;
}

StyleSheet::StyleSheet(std::initializer_list<std::pair<std::string, COLORREF>> properties) {
    for (auto& entry : properties) {
        this->properties[entry.first] = entry.second;
    }
}

StyleSheet::StyleSheet() {
    properties = {};
}