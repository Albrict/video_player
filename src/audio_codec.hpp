#pragma once
#include "codec_context.hpp"
#include "format_context.hpp"
#include "codec.hpp"

namespace VP {
    class AudioCodec final : public CodecContext {
    public:
        explicit AudioCodec(FormatContext &format_context)
            : CodecContext(format_context.getAudioCodec(), format_context.audioStreamCodecParams()) {}
        AudioCodec(AudioCodec &&other) noexcept
            : CodecContext(std::move(other))
        { *this = std::move(other); }

        ~AudioCodec() = default;

        AudioCodec &operator=(AudioCodec &&rhs) noexcept = default;

        [[nodiscard]] int sampleRate() const noexcept
        { return m_codec_ctx->sample_rate; }
        [[nodiscard]] int channels() const noexcept
        { return m_codec_ctx->ch_layout.nb_channels; }
        [[nodiscard]] AVChannelLayout channelLayout() const noexcept
        { return m_codec_ctx->ch_layout; }
        [[nodiscard]] AVSampleFormat sampleFormat() const noexcept
        { return m_codec_ctx->sample_fmt; }
    };
}
