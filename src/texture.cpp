#include "texture.hpp"
#include "renderer.hpp"
#include <cassert>
#include <iostream>
#include <stdexcept>

using namespace VP;

Texture::Texture(Renderer &render, const Uint32 format, const int access, const int w, const int h)
{ 
    m_texture = render.createTexture(format, access, w, h); 
    if (!m_texture)
        throw std::runtime_error(SDL_GetError());
}

Texture::Texture(Renderer &render, Texture &other)
{
    int                 width                 {};
    int                 height                {};
    Uint32              format                {}; 
    SDL_BlendMode       blend_mode            {};
    SDL_Texture        *current_render_target {};
    
    SDL_QueryTexture(other.m_texture, &format, nullptr, &width, &height);
    SDL_GetTextureBlendMode(other.m_texture, &blend_mode);

    current_render_target = render.getCurrentRenderTarget();
    m_texture             = render.createTexture(format, SDL_TEXTUREACCESS_TARGET, width, height);

    SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_NONE);
    render.setRenderTarget(m_texture);
    render.copy(other, nullptr, nullptr);
    SDL_SetTextureBlendMode(m_texture, blend_mode);

    render.setRenderTarget(current_render_target);

    m_width  = other.m_width;
    m_height = other.m_height;
}

void Texture::updateYUV(const SDL_Rect *rect, const Uint8 *y_plane, const int y_pitch, 
                const Uint8* u_plane, const int u_pitch, const Uint8 *v_plane, const int v_pitch)
{
    const int result = SDL_UpdateYUVTexture(m_texture, rect, y_plane, y_pitch, u_plane, u_pitch, v_plane, v_pitch);
    if (result < 0)
        std::cerr << SDL_GetError() << '\n';
}
