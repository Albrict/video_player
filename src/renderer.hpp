#pragma once
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include "def.hpp"

namespace VP {
    class Renderer {
    public:
        Renderer(Window &window, const int index = -1, Uint32 flags = 0);
        ~Renderer();
        
        void clear();
        void copy(Texture *texture, const SDL_Rect *src_rect = nullptr, const SDL_Rect *dst_rect = nullptr);
        void present() noexcept;
        SDL_Renderer *SDLRenderer() const noexcept
        { return m_render; }
    private:
        SDL_Renderer *m_render = nullptr;
    };
}
