#pragma once
#include "packet.hpp"
#include "video_frame_handler.hpp"
#include "audio_frame_handler.hpp"

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libswscale/swscale.h>
}

#include "format_context.hpp"

namespace VP {
    // Video object. Throws exception on construct failure
    class Video final {
    public:
        explicit Video(const char *path_to_file, Renderer &render);
        ~Video() = default;
        
        [[nodiscard]] FormatContext::FrameType readFrame();
        void renderFrame(Renderer &render);
        void playFrame();

        void play(Renderer &render);
        void pause();
        void unpause();

        Video &operator=(Video &&rhs) noexcept
        { 
            if (this != &rhs) {
                m_packet              = std::move(rhs.m_packet);
                m_format_ctx          = std::move(rhs.m_format_ctx);
            }
            return *this;
        }

        const FormatContext &getFormatContext() const noexcept
        { return m_format_ctx; }

        FormatContext &getFormatContext() noexcept
        { return m_format_ctx; }
    private:
        void renderFrame();
        void playSamples();
    private:
        FormatContext     m_format_ctx;         
        VideoFrameHandler m_video_frame_handler;
        AudioFrameHandler m_audio_frame_handler; 
        Packet            m_packet;
        bool              m_is_playing {true};
    };
}
