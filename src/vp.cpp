#include <SDL.h>
#include "vp.hpp"

extern "C" {
    #include <libavdevice/avdevice.h>
}

bool VP::init() noexcept
{
    int sdl_init_result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
    avdevice_register_all();

    if (sdl_init_result < 0) {
        SDL_Log("%s\n", SDL_GetError());
        return false;
    }
    return true;
}

void VP::close()
{
    SDL_Quit();
}
