#include "helper_functions.hpp"
#include "shgui.hpp"
#include "button.hpp"
#include <memory>

ShGUI::Button::Button(const Rectangle &rect, const Color color, const char *text)
    : Widget(rect, color),
    m_saved_button_color(color),
    m_saved_label_color(BLACK),
    m_button_hover_color(ColorBrightness(color, 0.2f)),
    m_label_hover_color(ColorBrightness(BLACK, 0.2f)),
    m_pressed_button_color(ColorBrightness(color, 0.4f)),
    m_pressed_label_color(ColorBrightness(BLACK, 0.4f)),
    m_label(nullptr),
    m_hover(false),
    m_pressed(false) 
{
    Rectangle label_rect = rect;
    label_rect.x += label_rect.width / 2.f - MeasureText(text, getFontSize()) / 2.f;
    label_rect.y += label_rect.height / 2.f - getFontSize() / 2.f;
    if (text)
        m_label = std::make_unique<Label>(label_rect, m_saved_label_color, text);
}


void ShGUI::Button::proccessEvents()
{
    if (ShGUI::isMouseOnWidget(*this)) {
        m_hover = true;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            m_pressed = true;
        } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            if (Widget::isCallbackSet())
                Widget::executeCallback();
        }
    } else {
        m_hover = false;
    }
}

void ShGUI::Button::update()
{
    if (m_pressed) {
        Widget::setColor(m_pressed_button_color);
        m_label->setColor(m_pressed_label_color);
        m_pressed = false;
    } else if (m_hover && !m_pressed) {
        Widget::setColor(m_button_hover_color);
        m_label->setColor(m_label_hover_color);
    } else {
        Widget::setColor(m_saved_button_color);
        m_label->setColor(m_saved_label_color);
    }
}

void ShGUI::Button::draw() const
{
    DrawRectangleRounded(Widget::getRect(), getRoundness(), 0, Widget::getColor());
    if (m_label)
        m_label->draw();
}
