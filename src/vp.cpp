#include <SDL.h>
#include "vp.hpp"

extern "C" {
    #include <libavdevice/avdevice.h>
}

namespace {
    bool running = false;
}

bool VP::init() noexcept
{
    int sdl_init_result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
    avdevice_register_all();

    if (sdl_init_result < 0) {
        SDL_Log("%s\n", SDL_GetError());
        return false;
    }
    running = true;
    return true;
}

bool VP::isRunning() noexcept
{ 
    return running; 
}

void VP::stopRunning() noexcept
{
    running = false;
}
