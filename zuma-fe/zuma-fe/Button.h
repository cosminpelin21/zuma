#pragma once
#include "raylib.h"
#include <string>

class Button {
private:
    Rectangle m_bounds;
    std::string m_label;
    Color m_baseColor;
    Color m_hoverColor;
    int m_fontSize;

public:
    Button(Rectangle bounds, const std::string& label, Color baseColor, Color hoverColor, int fontSize = 24);

    bool isClicked() const;
    void render() const;
};