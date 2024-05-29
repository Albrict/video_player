#pragma once
#include "renderer.hpp"
#include "sws_context.hpp"
#include "video_codec.hpp"
#include "video_frame.hpp"
#include "audio_frame_handler.hpp"
#include "packet.hpp"
#include <queue>
#include <SDL_events.h>

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libswscale/swscale.h>
}

#include "format_context.hpp"

namespace VP {
    // Video object. Throws exception on construct failure
    class Video final {
    public:
        explicit Video(const char *path_to_file, Window &window);
        ~Video();
        void play();

        const FormatContext &getFormatContext() const noexcept
        { return m_format_ctx; }

        FormatContext &getFormatContext() noexcept
        { return m_format_ctx; }
    private:
        enum Events {
           REFRESH_EVENT = SDL_USEREVENT
        };

        static Uint32 refreshTimer(Uint32 interval, void *data);
        void scheduleRefresh(const int delay); 
        void renderFrame();
        double synchronizeVideo(Frame &frame, double pts);
    private:
        FormatContext      m_format_ctx;         
        Renderer           m_render;
        VideoCodec         m_video_codec;
        VideoFrame         m_frame;
        VideoFrame         m_dest_frame;
        SwsContext         m_sws_context;
        AudioFrameHandler  m_audio_frame_handler; 
        std::queue<Packet> m_video_queue;
        bool               m_is_playing;
        bool               m_is_rendered;
        bool               m_is_updated;
        int                m_timer; 
        double             m_frame_timer;
        double             m_frame_last_pts;
        double             m_frame_last_delay;
        double             m_video_clock;
        double             m_audio_last_pts;
        double             m_audio_last_delay;
    };
}
