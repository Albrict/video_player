#include "texture.hpp"
#include "renderer.hpp"
#include <cassert>
#include <iostream>
#include <stdexcept>

VP::Texture::Texture(Renderer &render, const Uint32 format, const int access, const int w, const int h)
{ 
    m_texture = SDL_CreateTexture(render.SDLRenderer(), format, access, w, h);
    if (!m_texture)
        throw std::runtime_error(SDL_GetError());
}

void VP::Texture::updateYUV(const SDL_Rect *rect, const Uint8 *y_plane, const int y_pitch, 
                const Uint8* u_plane, const int u_pitch, const Uint8 *v_plane, const int v_pitch)
{
    const int result = SDL_UpdateYUVTexture(m_texture, rect, y_plane, y_pitch, u_plane, u_pitch, v_plane, v_pitch);
    if (result < 0)
        std::cerr << SDL_GetError() << '\n';
}
