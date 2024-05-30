#pragma once
#include "frame.hpp"

namespace VP {
    class AudioFrame final : public Frame {
    public:
        AudioFrame() = default;
        ~AudioFrame() = default;

        [[nodiscard]] int getChannels() const noexcept 
        { return m_frame->ch_layout.nb_channels; }
        [[nodiscard]] int getSampleRate() const noexcept
        { return m_frame->sample_rate; }
        [[nodiscard]] int getNumberOfSamples() const noexcept
        { return m_frame->nb_samples; }
    };
}
