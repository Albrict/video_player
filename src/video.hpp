#pragma once
#include <SDL_render.h>
#include <sys/types.h>
#include "codec_context.hpp"
#include "sws_context.hpp"
#include "frame.hpp"
#include "packet.hpp"

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

        Video(Video &&other) noexcept 
            : m_packet(std::move(other.m_packet)),
            m_format_ctx(std::move(other.m_format_ctx)),
            m_video_codec_context(std::move(other.m_video_codec_context)),
            m_sws_context(std::move(other.m_sws_context)),
            m_frame(std::move(other.m_frame)),
            m_dest_frame(std::move(other.m_dest_frame))
        {}

        // Get one frame from video stream.
        // If stream is ended - returns nullptr
        Texture *getFrame();

        Video &operator=(Video &&rhs) noexcept
        { 
            if (this != &rhs) {
                m_packet              = std::move(rhs.m_packet);
                m_format_ctx          = std::move(rhs.m_format_ctx);
                m_video_codec_context = std::move(rhs.m_video_codec_context);
                m_sws_context         = std::move(rhs.m_sws_context);
                m_frame               = std::move(rhs.m_frame);
                m_dest_frame          = std::move(rhs.m_dest_frame);
            }
            return *this;
        }

        const FormatContext &getFormatContext() const noexcept
        { return m_format_ctx; }

        FormatContext &getFormatContext() noexcept
        { return m_format_ctx; }
    private:
        Packet        m_packet;
        FormatContext m_format_ctx;         
        CodecContext  m_video_codec_context;
        SwsContext    m_sws_context;
        Frame         m_frame;
        Frame         m_dest_frame;
    };
}
