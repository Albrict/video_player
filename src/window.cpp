#include "window.hpp"
#include "raylib.h"

using namespace VideoPlayer;

Window::Window(const unsigned int width, const unsigned int height, const char *title)
    : m_width(width),
    m_height(height),
    m_title(title)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);
    InitWindow(width, height, title);
}
