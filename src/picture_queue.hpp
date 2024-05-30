#pragma once
#include <queue>
#include "video_frame.hpp"

struct SDL_mutex;
struct SDL_cond;

namespace VP {
    class PictureQueue {
    public:
        enum class Result {
            PROGRAM_IS_DONE,
            SUCCESS
        };

        PictureQueue();
        ~PictureQueue();

        Result putFrameInQueue(Frame &frame, const double pts);
    private:
        const int video_picture_queue_size = 1; 
        struct Picture {
            Texture texture;
            double  pts  {};
        };
        std::queue<Texture> m_picture_queue {};
        SDL_mutex          *m_mutex         {};
        SDL_cond           *m_cond          {};
    };
}
