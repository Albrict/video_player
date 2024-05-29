#include "video.hpp"
#include "window.hpp"
#include "vp.hpp"

int main(int argc, char **argv)
{
    if (argc != 2) 
        return 1;
    const bool result = VP::init();
    if (result == false)
        return 1;
    VP::Window window(SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, "Title");
    VP::Video video(argv[1], window);
    video.play();
    VP::close();
}
