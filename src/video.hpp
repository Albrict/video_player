#pragma once
#include <SDL_render.h>
#include <sys/types.h>

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
}

namespace VP {
    // Video object. Throws exception on construct failure
    class Video {
    public:
        Video(const char *path_to_file, SDL_Renderer *render);
        ~Video();

        // Get one frame from video stream.
        // If stream is ended - returns nullptr
        SDL_Texture *getFrame();
    private:
        const AVCodec   *m_codec            = nullptr;
        AVCodecContext  *m_codec_ctx        = nullptr;
        AVFormatContext *m_format_ctx       = nullptr;
        SwsContext      *m_sws_ctx          = nullptr;
        AVFrame         *m_frame            = nullptr;
        AVPacket        m_packet              = {0};

        SDL_Texture     *m_texture            = nullptr;
        
        int             m_uv_pitch            = 0;
        int             m_video_stream         = 0;
        Uint8           *m_y_plane            = nullptr;
        Uint8           *m_u_plane            = nullptr;
        Uint8           *m_v_plane            = nullptr;

    };
}
