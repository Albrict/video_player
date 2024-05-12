#include "audio_device.hpp"
#include "error.hpp"
#include "swr_context.hpp"
#include <libavutil/samplefmt.h>
#include <stdexcept>

extern "C" {
    #include <libswresample/swresample.h>
    #include <libavutil/frame.h>
}

using namespace VP;

AudioDevice::AudioDevice(const SDL_AudioSpec *desired, SDL_AudioSpec *obtained, 
            const int allowed_changes, const char *device_name)
    : m_id(SDL_OpenAudioDevice(device_name, 0, desired, obtained, allowed_changes))
    
{
    m_frame = av_frame_alloc();
    if (m_id == 0)
        throw std::runtime_error(SDL_GetError());
}

AudioDevice::~AudioDevice()
{
    av_frame_free(&m_frame);
    SDL_CloseAudioDevice(m_id);
}

void AudioDevice::queueAudioFromFrame(Frame &frame, SwrContext &swr_context)
{
    int linesize = 0;
    const int delay       = swr_context.getDelay(frame) + frame.getNumberOfSamples();
    int dst_samples       = frame.getChannels();
    dst_samples          *= av_rescale_rnd(delay, 44100, frame.getSampleRate(), AV_ROUND_UP);
    uint8_t**   buffer    = nullptr;
    int result            = av_samples_alloc_array_and_samples(&buffer, nullptr, frame.getChannels(), frame.getNumberOfSamples(), AV_SAMPLE_FMT_S16, 0);
    check_libav_return_value(result);
    
    dst_samples = frame.getChannels() * swr_context.convert(buffer, dst_samples, (const uint8_t**)frame.getAVFrame()->data, frame.getNumberOfSamples());
    int res_size = av_samples_get_buffer_size(&linesize, frame.getChannels(), frame.getNumberOfSamples(), AV_SAMPLE_FMT_S16, 1);
    SDL_QueueAudio(m_id, buffer[0], res_size);
    av_freep(&buffer[0]);
    av_freep(&buffer);
}
