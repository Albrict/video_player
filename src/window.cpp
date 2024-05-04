#include "window.hpp"
#include "SDL_video.h"
#include <stdexcept>

VP::Window::Window(const int x, const int y, const int width, const int height, const char *title)
{
    m_window = SDL_CreateWindow(title, x, y, width, height, SDL_WINDOW_SHOWN);
    if (!m_window)
        throw std::runtime_error(SDL_GetError());
}

