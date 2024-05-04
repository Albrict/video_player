#include <cassert>
#include <stdexcept>

#include "renderer.hpp"
#include "texture.hpp"
#include "window.hpp"

VP::Renderer::Renderer(Window &window, const int index, Uint32 flags)
{ 
    m_render = SDL_CreateRenderer(window.SDLWindow(), index, flags); 
    if (!m_render) 
        throw std::runtime_error(SDL_GetError()); 
}

VP::Renderer::~Renderer()
{
    SDL_DestroyRenderer(m_render);
}

void VP::Renderer::clear()
{
    const int result = SDL_RenderClear(m_render);
    assert(result == 0);
}


void VP::Renderer::copy(Texture *texture, const SDL_Rect *src_rect, const SDL_Rect *dst_rect)
{
    const int result = SDL_RenderCopy(m_render, texture->SDLTexture(), src_rect, dst_rect);
    assert(result == 0);
}

void VP::Renderer::present() noexcept
{
    SDL_RenderPresent(m_render);
}
