#include "Button.h"

Button::Button(Rectangle bounds, const std::string& label, Color baseColor, Color hoverColor, int fontSize)
    : m_bounds(bounds), m_label(label), m_baseColor(baseColor), m_hoverColor(hoverColor), m_fontSize(fontSize) {
}

bool Button::isClicked() const {
    return CheckCollisionPointRec(GetMousePosition(), m_bounds) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void Button::render() const {
    bool hovered = CheckCollisionPointRec(GetMousePosition(), m_bounds);
    Rectangle drawRect = m_bounds;

    if (hovered) {
        drawRect.x -= 4.0f;
        drawRect.y -= 2.0f;
        drawRect.width += 8.0f;
        drawRect.height += 4.0f;
    }

    DrawRectangleRounded(drawRect, 0.3f, 8, hovered ? m_hoverColor : m_baseColor);
    DrawRectangleRoundedLinesEx(drawRect, 0.3f, 8, 2.5f, WHITE);

    int textW = MeasureText(m_label.c_str(), m_fontSize);
    DrawText(m_label.c_str(),
        static_cast<int>(drawRect.x + (drawRect.width - textW) / 2),
        static_cast<int>(drawRect.y + (drawRect.height - m_fontSize) / 2),
        m_fontSize, WHITE);
}