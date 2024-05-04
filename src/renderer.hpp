#pragma once
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_stdinc.h>
#include "def.hpp"

namespace VP {
    class Renderer final {
    public:
        explicit Renderer(Window &window, const int index = -1, const Uint32 flags = 0);
        ~Renderer();
        
        void clear();
        void copy(Texture &texture, const SDL_Rect *src_rect = nullptr, const SDL_Rect *dst_rect = nullptr);
        void copy(Texture *texture, const SDL_Rect *src_rect = nullptr, const SDL_Rect *dst_rect = nullptr)
        { copy(*texture, src_rect, dst_rect); }
        void present() noexcept;

        void setRenderTarget(Texture &texture) const noexcept;
        void setRenderTarget(SDL_Texture *texture) const noexcept
        { SDL_SetRenderTarget(m_render, texture); }

        SDL_Texture *createTexture(const Uint32 format, const int access, const int width, const int height) const noexcept
        { return SDL_CreateTexture(m_render, format, access, width, height); }

        const SDL_Texture *getCurrentRenderTarget() const noexcept
        { return SDL_GetRenderTarget(m_render); }
        SDL_Texture *getCurrentRenderTarget() noexcept
        { return SDL_GetRenderTarget(m_render); }

        SDL_Renderer *SDLRenderer() const noexcept
        { return m_render; }
    private:
        SDL_Renderer *m_render = nullptr;
    };
}
