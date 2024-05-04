#pragma once
#include <SDL_render.h>

namespace VP {
    class Window final {
    public:
        explicit Window(const int x, const int y, const int width, const int height, const char *title = nullptr);
        ~Window()
        { SDL_DestroyWindow(m_window); }
        
        const SDL_Surface *surface() const noexcept
        { return SDL_GetWindowSurface(m_window); }
        SDL_Window *SDLWindow() const noexcept
        { return m_window; }
    private:
        SDL_Window *m_window {};
    };
}
