#pragma once
#include <SDL_audio.h>
#include "def.hpp"

struct SDL_AudioDevice;
struct AVFrame;

namespace VP {
    class AudioDevice final {
    public:
        explicit AudioDevice(const SDL_AudioSpec *desired, SDL_AudioSpec *obtained, 
                    const int allowed_changes = SDL_AUDIO_ALLOW_ANY_CHANGE, const char *device_name = nullptr);
        ~AudioDevice();

        void pause() noexcept
        { SDL_PauseAudioDevice(m_id, 1); }
        void unpause() noexcept
        { SDL_PauseAudioDevice(m_id, 0); }

        void queueAudioFromFrame(Frame &frame, SwrContext &swr_context);
    private:
        AVFrame          *m_frame        {};
        SDL_AudioDeviceID m_id           {};
    };
}
