#include "label.hpp"
#include "shgui.hpp"
#include <raylib.h>

ShGUI::Label::Label(const Rectangle &rect, const Color color, const char *text) noexcept
    : Widget(rect, color),
    m_text(text),
    m_font_size(ShGUI::getFontSize()),
    m_spacing(rect.width / ShGUI::getFontSize()) {}

void ShGUI::Label::draw() const
{
    const auto &rect = Widget::getRectRef(); 
    DrawTextPro(GetFontDefault(), m_text, {rect.x, rect.y}, {0.f, 0.f}, 0.f, m_font_size, m_spacing, Widget::getColor());
}
