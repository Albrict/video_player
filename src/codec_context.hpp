#pragma once
#include "def.hpp"

extern "C" {
    #include <libavutil/channel_layout.h>
    #include <libavcodec/avcodec.h>
}

namespace VP {
    class CodecContext final {
    public:
        explicit CodecContext(const Codec &codec, AVCodecParameters *parameters);
        CodecContext(CodecContext &&other) noexcept
        { *this = std::move(other); }
        ~CodecContext();

        CodecContext &operator=(CodecContext &&rhs) noexcept;

        [[nodiscard]] int width() const noexcept 
        { return m_codec_ctx->width; }
        [[nodiscard]] int height() const noexcept 
        { return m_codec_ctx->height; }
        [[nodiscard]] AVPixelFormat pixelFormat() const noexcept 
        { return m_codec_ctx->pix_fmt; }
        
        [[nodiscard]] int sampleRate() const noexcept
        { return m_codec_ctx->sample_rate; }
        [[nodiscard]] int channels() const noexcept
        { return m_codec_ctx->ch_layout.nb_channels; }
        [[nodiscard]] AVChannelLayout channelLayout() const noexcept
        { return m_codec_ctx->ch_layout; }
        [[nodiscard]] AVSampleFormat sampleFormat() const noexcept
        { return m_codec_ctx->sample_fmt; }

        void sendPacket(const Packet &packet);
        int  receiveFrame(Frame &frame);

        const AVCodecContext *getAVCodecContext() const noexcept
        { return m_codec_ctx; }
        AVCodecContext *getAVCodecContext() noexcept
        { return m_codec_ctx; }

        CodecContext(const CodecContext &other) = delete;
        CodecContext &operator=(const CodecContext &rhs) = delete;
    private:
        AVCodecContext *m_codec_ctx {};
    };
}
