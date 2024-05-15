#include <iostream>
#include <SDL.h>
#include "format_context.hpp"
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
        VP::Window window(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, "Video player");
        VP::Renderer render(window);

        VP::Video video(argv[1], render);

        SDL_Event event {};
        while (VP::isRunning()) {
            const auto type = video.readFrame(); 
            switch(type) {
            using enum VP::FormatContext::FrameType;
            case VIDEO_STREAM:
                render.clear();
                video.renderFrame(render);
                render.present();
                break;
            case AUDIO_STREAM:
                video.playFrame();
                break;
            case END_OF_STREAM:
                VP::stopRunning();
                break;
            default:
                break;
            }

            SDL_PollEvent(&event);
            switch (event.type) {
            case SDL_QUIT:
                VP::stopRunning();
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
