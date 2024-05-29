#pragma once
#include <SDL_audio.h>
#include "audio_frame.hpp"
#include "swr_context.hpp"

struct SDL_AudioDevice;

namespace VP {
    class AudioFrameHandler final {
    public:
        explicit AudioFrameHandler(FormatContext &format_context);
        ~AudioFrameHandler();
        
        void playFrame(Packet &packet);
        void pause() const noexcept
        { SDL_PauseAudioDevice(m_id, 1); }
        void unpause() const noexcept
        { SDL_PauseAudioDevice(m_id, 0); }
    private:
        static void audioCallback(void *userdata, Uint8 *stream, int length);
    private:
        AudioCodec        m_audio_codec;
        SwrContext        m_swr_context;
        AudioFrame        m_audio_frame;
        SDL_AudioDeviceID m_id;

        AVRational        m_audio_timebase;
    };
}
