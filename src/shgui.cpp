#include "shgui.hpp"

namespace ShGUI {
    float g_roundness = 0.5f;
    unsigned int g_font_size = 28;
}

void ShGUI::setRoundness(const float roundness) noexcept
{
   g_roundness = roundness; 
}

float ShGUI::getRoundness() noexcept
{
    return g_roundness;
}

void ShGUI::setFontSize(const unsigned int font_size) noexcept
{
    g_font_size = font_size;
}

unsigned int ShGUI::getFontSize() noexcept
{
    return g_font_size;
}
