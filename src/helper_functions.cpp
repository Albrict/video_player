#include "widget.hpp"
#include "helper_functions.hpp"

bool ShGUI::isMouseOnWidget(const Widget &widget)
{
    return CheckCollisionPointRec(GetMousePosition(), widget.getRect());
}
