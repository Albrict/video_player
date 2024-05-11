#include "texture.hpp"
#include "error.hpp"
#include "renderer.hpp"
#include <cassert>
#include <stdexcept>

using namespace VP;

Texture::Texture(Renderer &render, const Uint32 format, const SDL_TextureAccess access, const int w, const int h)
{ 
    assert(w > 0 && h > 0 && "Width or height are invalid!");
    m_texture = render.createTexture(format, access, w, h); 
    if (!m_texture)
        throw std::runtime_error(SDL_GetError());
}

Texture::Texture(Renderer &render, Texture &other)
{
    int                 width                 {};
    int                 height                {};
    int                 result                {};
    Uint32              format                {}; 
    SDL_BlendMode       blend_mode            {};
    SDL_Texture        *current_render_target {};
    
    result = SDL_QueryTexture(other.m_texture, &format, nullptr, &width, &height);
    check_sdl_return_value(result);

    result = SDL_GetTextureBlendMode(other.m_texture, &blend_mode);
    check_sdl_return_value(result); 

    current_render_target = render.getCurrentRenderTarget();
    m_texture             = render.createTexture(format, SDL_TEXTUREACCESS_TARGET, width, height);

    result = SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_NONE);
    check_sdl_return_value(result);

    render.setRenderTarget(m_texture);
    render.copy(other, nullptr, nullptr);

    result = SDL_SetTextureBlendMode(m_texture, blend_mode);
    check_sdl_return_value(result);

    render.setRenderTarget(current_render_target);

    m_width  = other.m_width;
    m_height = other.m_height;
}

void Texture::updateYUV(const SDL_Rect *rect, const Uint8 *y_plane, const int y_pitch, 
                const Uint8* u_plane, const int u_pitch, const Uint8 *v_plane, const int v_pitch)
{
    assert(y_plane != nullptr && u_plane != nullptr && v_plane != nullptr && "Planes are nullptr!");
    assert(y_pitch > 0 && u_pitch > 0 && v_pitch > 0 && "Pitches are negative!");

    const int result = SDL_UpdateYUVTexture(m_texture, rect, y_plane, y_pitch, u_plane, u_pitch, v_plane, v_pitch);
    check_sdl_return_value(result);
}
