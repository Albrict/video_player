#include "window.hpp"
#include "SDL_video.h"
#include <cassert>
#include <stdexcept>

using namespace VP;

Window::Window(const int x, const int y, const int width, const int height, const char *title)
{
    SDL_DisplayMode display_mode {};
    SDL_GetCurrentDisplayMode(0, &display_mode);
    assert(x > 0 && y > 0 && "x and y coordinates are invalid!");
    assert(width > 0 && height > 0 && "Width and height are invalid!");
    assert(width < display_mode.w && height < display_mode.w && "Width and height bigger than screen!");

    m_window = SDL_CreateWindow(title, x, y, width, height, SDL_WINDOW_SHOWN);
    if (!m_window)
        throw std::runtime_error(SDL_GetError());
}

