#pragma once
#include "def.hpp"

extern "C" {
    #include <libavcodec/avcodec.h>
}

namespace VP {
    class CodecContext {
    public:
        CodecContext(Codec codec, AVCodecParameters *parameters);
        ~CodecContext();

        [[nodiscard]] int width() const noexcept 
        { return m_codec_ctx->width; }
        [[nodiscard]] int height() const noexcept 
        { return m_codec_ctx->height; }
        [[nodiscard]] AVPixelFormat pixelFormat() const noexcept 
        { return m_codec_ctx->pix_fmt; }

        void sendPacket(AVPacket *packet);
        void receiveFrame(Frame &frame);
    private:
        AVCodecContext *m_codec_ctx {};
    };
}
