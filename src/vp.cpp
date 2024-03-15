#include <SDL.h>
#include "vp.hpp"

bool VP::init()
{
    int sdl_init_result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_EVENTS);
    if (sdl_init_result < 0) {
        SDL_Log("%s\n", SDL_GetError());
        return false;
    }
    return true;
}
