#pragma once
#include <SDL.h>

namespace VP {
    class Window {
    public:
        Window(const int x, const int y, const int width, const int height, const char *title = nullptr);
        ~Window()
        { SDL_DestroyWindow(m_window); }
        
        const SDL_Surface *surface()
        { return SDL_GetWindowSurface(m_window); }
     
    private:
        SDL_Window *m_window = nullptr;
    };
}
