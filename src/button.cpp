#include "helper_functions.hpp"
#include <raylib.h>
#include "button.hpp"

ShGUI::Button::Button(const Rectangle &rect, const Color color, const char *text)
    : Widget(rect, color),
    m_text(text),
    m_saved_color(color),
    m_pressed_color(ColorBrightness(color, 0.4f)),
    m_change_color(false) {}


void ShGUI::Button::proccessEvents()
{
    if (ShGUI::isMouseOnWidget(*this)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            m_change_color = true;
        } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            if (Widget::isCallbackSet())
                Widget::executeCallback();
        }
    }
}

void ShGUI::Button::update()
{
    if (m_change_color) {
        Widget::changeColor(m_pressed_color);
        m_change_color = false;
    } else {
        Widget::changeColor(m_saved_color);
    }
}

void ShGUI::Button::draw() const
{
    DrawRectangleRounded(Widget::getRect(), 0.3f, 0, Widget::getColor());
}
