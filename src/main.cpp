#include <iostream>
#include <SDL.h>
#include "vp.hpp"
#include "window.hpp"
#include "video.hpp"

int main(int argc, char *argv[]) 
{
    if (argc < 2) {
		fprintf(stderr, "Usage: test <file>\n");
		exit(1);
	}
        
    // Init SDL
    if (!VP::init())
        exit(1);
    try {
        VP::Window window(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, "Video player");
        VP::Video video(argv[1], window);
        video.play();
    } catch(std::exception &ex) {
        std::cout << ex.what(); 
        return -1;
    }
    VP::close();
    return 0;
}
