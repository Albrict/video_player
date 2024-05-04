#pragma once
#include "def.hpp"

extern "C" {
    #include <libavcodec/avcodec.h>
}

namespace VP {
    class CodecContext final {
    public:
        explicit CodecContext(Codec codec, AVCodecParameters *parameters);
        CodecContext(CodecContext &&other) noexcept
        {
            m_codec_ctx       = other.m_codec_ctx; 
            other.m_codec_ctx = nullptr;
        }
        ~CodecContext();

        CodecContext &operator=(CodecContext &&rhs) noexcept
        { return *this = CodecContext(std::move(rhs)); }

        [[nodiscard]] int width() const noexcept 
        { return m_codec_ctx->width; }
        [[nodiscard]] int height() const noexcept 
        { return m_codec_ctx->height; }
        [[nodiscard]] AVPixelFormat pixelFormat() const noexcept 
        { return m_codec_ctx->pix_fmt; }

        void sendPacket(const AVPacket *packet);
        int  receiveFrame(Frame &frame);


        CodecContext(const CodecContext &other) = delete;
        CodecContext &operator=(const CodecContext &rhs) = delete;
    private:
        AVCodecContext *m_codec_ctx {};
    };
}
