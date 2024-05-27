#pragma once
#include "renderer.hpp"
#include "video_frame_handler.hpp"
#include "audio_frame_handler.hpp"
#include "packet.hpp"
#include <queue>

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
        ~Video() = default;
        void play();

        const FormatContext &getFormatContext() const noexcept
        { return m_format_ctx; }

        FormatContext &getFormatContext() noexcept
        { return m_format_ctx; }
    private:
        void renderFrame();
        void playSamples();
    private:
        FormatContext      m_format_ctx;         
        Renderer           m_render;
        VideoFrameHandler  m_video_frame_handler;
        AudioFrameHandler  m_audio_frame_handler; 
        std::queue<Packet> m_video_queue;
        bool               m_is_playing;
    };
}
