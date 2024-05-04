#include <iostream>
#include <SDL.h>
#include "vp.hpp"
#include "renderer.hpp"
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
        VP::Window window(SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, "Video player");
        VP::Renderer render(window);

        VP::Video video(argv[1], render);

        SDL_Event event {};
        bool running = true;
        while (running) {
            auto texture = video.getFrame();
            if (texture == nullptr) {
                running = false;
                break; 
            }
            render.clear();
            render.copy(texture);
            render.present();

            SDL_PollEvent(&event);
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            default:
                break;
            }
        }
    } catch(std::exception &ex) {
        std::cout << ex.what(); 
        return -1;
    }
    SDL_Quit();
    return 0;
}
