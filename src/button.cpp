#include "helper_functions.hpp"
#include "button.hpp"

ShGUI::Button::Button(const Rectangle &rect, const Color color, const char *text)
    : Widget(rect, color),
    m_text(text),
    m_saved_color(color),
    m_change_color(false) {}


void ShGUI::Button::proccessEvents()
{
    if (ShGUI::isMouseOnWidget(*this)) {
        m_change_color = true;
        const auto pressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        const auto has_callback = Widget::isCallbackSet();
        if (pressed) {
            if (has_callback)
                Widget::executeCallback();
        }
    }
}

void ShGUI::Button::update()
{
    if (m_change_color) {
        Widget::changeColor(RED);
        m_change_color = false;
    } else {
        Widget::changeColor(m_saved_color);
    }
}

void ShGUI::Button::draw() const
{
    DrawRectangleRec(Widget::getRect(), Widget::getColor());
}
