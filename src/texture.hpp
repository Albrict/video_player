#pragma once
#include <SDL_stdinc.h>
#include <SDL_render.h>
#include "def.hpp"

namespace VP {
    class Texture {
    public:
        Texture(Renderer &render, const Uint32 format, const int access, const int width, const int height);
        ~Texture()
        { SDL_DestroyTexture(m_texture); }
        
        SDL_Texture *SDLTexture() const noexcept
        { return m_texture; }

        void updateYUV(const SDL_Rect *rect, const Uint8 *y_plane, const int y_pitch, 
                        const Uint8* u_plane, const int u_pitch, const Uint8 *v_plane, const int v_pitch);;

        [[nodiscard]] int getWidth() const noexcept
        { return m_width; }
        [[nodiscard]] int getHeight() const noexcept
        { return m_height; }
    private:
        int          m_width   {};
        int          m_height  {};
        SDL_Texture *m_texture = nullptr;
    };
}
