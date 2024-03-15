#include "video.hpp"
#include <unistd.h>
#include <SDL.h>
extern "C" {
    #include <libavutil/avutil.h>
    #include <libavcodec/avcodec.h>
    #include <libavcodec/codec.h>
    #include <libavutil/imgutils.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
    #include <libswscale/swscale.h>
}

#include "vp.hpp"

int main(int argc, char *argv[]) {
    SDL_Event event;
    SDL_Window *screen;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    if (argc < 2) {
        fprintf(stderr, "Usage: test <file>\n");
        exit(1);
    }
    
    // Init SDL
    if (!VP::init()) {
        exit(1);
    }


    // Make a screen to put our video
    screen = SDL_CreateWindow(
            "FFmpeg Tutorial",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            1920,
            1200,
            0
        );

    if (!screen) {
        fprintf(stderr, "SDL: could not create window - exiting\n");
        exit(1);
    }

    renderer = SDL_CreateRenderer(screen, -1, 0);
    if (!renderer) {
        fprintf(stderr, "SDL: could not create renderer - exiting\n");
        exit(1);
    }
    VP::Video video("test.mkv", renderer);
    // Allocate a place to put our YUV image on that screen

    while (auto texture = video.getFrame()) {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        // Free the packet that was allocated by av_read_frame
        SDL_PollEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
            SDL_DestroyTexture(texture);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(screen);
            SDL_Quit();
            exit(0);
            break;
        default:
            break;
        }

    }

//    // Free the YUV frame
//    av_frame_free(&pFrame);
//    free(yPlane);
//    free(uPlane);
//    free(vPlane);
//
//    // Close the codec
//    avcodec_close(pCodecCtx);
//    avcodec_close(pCodecCtxOrig);
//
//    // Close the video file
//    avformat_close_input(&pFormatCtx);

    return 0;
}
