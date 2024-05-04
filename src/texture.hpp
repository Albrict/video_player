#pragma once
#include <SDL_stdinc.h>
#include <SDL_render.h>
#include "def.hpp"

namespace VP {
    class Texture final {
    public:
        explicit Texture(Renderer &render, const Uint32 format, const int access, const int width, const int height);
        Texture(Renderer &render, Texture &other);
        Texture(Renderer &render, Texture *other)
        { Texture(render, *other); }

        Texture(Renderer &render, Texture &&other) noexcept
        {
            m_texture = other.m_texture;
            m_width   = other.m_width;
            m_height  = other.m_height;

            other.m_texture = nullptr;
            other.m_width   = 0;
            other.m_height  = 0;
        }

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

        Texture &operator=(Texture &rhs)  = delete;
        Texture &operator=(Texture &&rhs) = delete;
    private:
        SDL_Texture *m_texture {};
        int          m_width   {};
        int          m_height  {};
    };
}
