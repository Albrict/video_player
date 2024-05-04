#pragma once
#include <SDL_render.h>
#include <sys/types.h>
#include "codec_context.hpp"
#include "sws_context.hpp"
#include "frame.hpp"

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libswscale/swscale.h>
}

#include "format_context.hpp"

namespace VP {
    // Video object. Throws exception on construct failure
    class Video {
    public:
        Video(const char *path_to_file, Renderer &render);
        ~Video() = default;

        // Get one frame from video stream.
        // If stream is ended - returns nullptr
        Texture *getFrame();
    private:
        AVPacket                           m_packet {};
        FormatContext                      m_format_ctx;         
        CodecContext                       m_video_codec_context;
        SwsContext                         m_sws_context;
        Frame                              m_frame;
        Frame                              m_dest_frame;
        int                                m_video_stream_id {};
    };
}
