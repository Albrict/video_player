#pragma once
#include "codec_context.hpp"
#include "format_context.hpp"
#include "codec.hpp"

extern "C" {
    #include <libavcodec/avcodec.h>
}

namespace VP {
    class VideoCodec final : public CodecContext {
    public:
        explicit VideoCodec(FormatContext &format_context)
            : CodecContext(format_context.getVideoCodec(), format_context.videoStreamCodecParams()) {}
        VideoCodec(VideoCodec &&other) noexcept
            : CodecContext(std::move(other))
        { *this = std::move(other); }

        ~VideoCodec() = default;

        VideoCodec &operator=(VideoCodec &&rhs) noexcept = default;
        
        [[nodiscard]] int width() const noexcept 
        { return CodecContext::m_codec_ctx->width; }
        [[nodiscard]] int height() const noexcept 
        { return CodecContext::m_codec_ctx->height; }
        [[nodiscard]] AVPixelFormat pixelFormat() const noexcept 
        { return CodecContext::m_codec_ctx->pix_fmt; }

        VideoCodec(const CodecContext &other) = delete;
        VideoCodec &operator=(const CodecContext &rhs) = delete;
    };
}
