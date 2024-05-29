#include "audio_frame_handler.hpp"
#include "error.hpp"
#include "packet.hpp"
#include "swr_context.hpp"
#include "vp.hpp"

extern "C" {
    #include <libswresample/swresample.h>
    #include <libavutil/frame.h>
}

using VP::AudioFrameHandler;

namespace {
    constexpr int max_audio_frame_size = 192000;
}
AudioFrameHandler::AudioFrameHandler(FormatContext &format_context)
    : m_audio_codec(format_context),
    m_swr_context(m_audio_codec, AV_SAMPLE_FMT_S16, m_audio_codec.sampleRate()),
    m_audio_frame(),
    m_id()
{
    const int audio_buffer_size = 1024;
    SDL_AudioSpec desired  {};
    SDL_AudioSpec obtained {};

    desired.freq     = m_audio_codec.sampleRate();
    desired.format   = AUDIO_S16;
    desired.channels = m_audio_codec.channels();
    desired.silence  = 0;

    m_id = SDL_OpenAudioDevice(nullptr, 0, &desired, &obtained, SDL_AUDIO_ALLOW_ANY_CHANGE);
    if (m_id == 0)
        throw std::runtime_error(SDL_GetError());
    unpause();
}

AudioFrameHandler::~AudioFrameHandler()
{
    SDL_CloseAudioDevice(m_id);
}


void AudioFrameHandler::playFrame(Packet &packet)
{
    m_audio_codec.sendPacket(packet);
    while (m_audio_codec.receiveFrame(m_audio_frame) >= 0) {
        int linesize          = 0;
        const int delay       = m_swr_context.getDelay(m_audio_frame) + m_audio_frame.getNumberOfSamples();
        int dst_samples       = m_audio_frame.getChannels();
        dst_samples          *= av_rescale_rnd(delay, 44100, m_audio_frame.getSampleRate(), AV_ROUND_UP);
        uint8_t**   buffer    = nullptr;
        int result            = av_samples_alloc_array_and_samples(&buffer, nullptr, m_audio_frame.getChannels(), m_audio_frame.getNumberOfSamples(), AV_SAMPLE_FMT_S16, 0);
        check_libav_return_value(result);
        dst_samples = m_audio_frame.getChannels() * m_swr_context.convert(buffer, dst_samples, (const uint8_t**)m_audio_frame.getAVFrame()->data, m_audio_frame.getNumberOfSamples());
        int res_size = av_samples_get_buffer_size(&linesize, m_audio_frame.getChannels(), m_audio_frame.getNumberOfSamples(), AV_SAMPLE_FMT_S16, 1);
        SDL_QueueAudio(m_id, buffer[0], res_size);
        av_freep(&buffer[0]);
        av_freep(&buffer);
    }
}
