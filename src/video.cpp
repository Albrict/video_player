#include "video.hpp"
#include "audio_device.hpp"
#include "format_context.hpp"
#include "frame.hpp"
#include "codec_context.hpp"
#include "vp.hpp"
#include "codec.hpp"
#include <cassert>
#include <cerrno>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>

extern "C" {
    #include <libavformat/avformat.h>
}

using VP::Video;
using VP::Texture;
using VP::CodecContext;
using VP::Packet;

namespace {
//    constexpr size_t max_audio_frame_size = 192000;
//    
//    int audio_decode_frame(CodecContext &codec_context, uint8_t *audio_buffer, const int buffer_size);
//    int audio_resampling(CodecContext &audio_decode_ctx, AVFrame *decoded_audio_frame, enum AVSampleFormat out_sample_fmt, int out_channels, int out_sample_rate, uint8_t * out_buf);
//    void audio_callback(void *userdata, Uint8 *stream, int length);
}

Video::Video(const char *path_to_file, Renderer &render)
    : m_format_ctx(path_to_file),
    m_video_codec_context(m_format_ctx.getVideoCodec(), m_format_ctx.videoStreamCodecParams()),
    m_audio_codec_context(m_format_ctx.getAudioCodec(), m_format_ctx.audioStreamCodecParams()),
    m_sws_context(m_video_codec_context, m_video_codec_context.width(), m_video_codec_context.height(), AV_PIX_FMT_YUV420P, SWS_BILINEAR),
    m_swr_context(m_audio_codec_context, AV_SAMPLE_FMT_S16, m_audio_codec_context.sampleRate()),
    m_frame(render, m_video_codec_context.width(), m_video_codec_context.height()),
    m_dest_frame(render, m_video_codec_context.width(), m_video_codec_context.height())
{
    SDL_AudioSpec wanted_spec {};
    SDL_AudioSpec spec        {};
    
    wanted_spec.freq     = m_audio_codec_context.sampleRate();
    wanted_spec.format   = AUDIO_S16SYS;
    wanted_spec.channels = m_audio_codec_context.channels();
    wanted_spec.silence  = 0;
    wanted_spec.callback = nullptr;
    wanted_spec.userdata = m_audio_codec_context.getAVCodecContext();

    m_audio = std::make_unique<AudioDevice>(&wanted_spec, &spec);
    m_audio->unpause(); 
}

Texture *Video::getFrame()
{
    const auto frame_result = m_format_ctx.getVideoFrame(m_packet);
    int receive_frame_result = 0;
    switch(frame_result) {
    using enum FormatContext::VideoFrameReturnValue;
    case END_OF_STREAM:
        return nullptr;
    case VIDEO_STREAM:
        do {
            m_video_codec_context.sendPacket(m_packet);
            receive_frame_result = m_video_codec_context.receiveFrame(m_frame);
            m_sws_context.scale(m_video_codec_context, m_frame, m_dest_frame);
            m_packet.unref();
        } while (receive_frame_result == EAGAIN);
        return m_dest_frame.getTexture();
    case AUDIO_STREAM:
        do {
            m_audio_codec_context.sendPacket(m_packet);
            receive_frame_result = m_audio_codec_context.receiveFrame(m_frame);
            m_audio->queueAudioFromFrame(m_frame, m_swr_context);
            m_packet.unref();
        } while (receive_frame_result == EAGAIN);
        return m_dest_frame.getTexture();
    case ERROR:
        return nullptr;
    case OTHER_STREAM:
        m_packet.unref();
        return m_dest_frame.getTexture();
    };
}

namespace {
//    void audio_callback(void *userdata, Uint8 *stream, int length)
//    {
//        CodecContext *codec_context = static_cast<CodecContext*>(userdata);
//        int          length_1       = -1;
//        int          audio_length   = -1;
//        static uint8_t audio_buffer[(max_audio_frame_size * 3) / 2];
//        static unsigned int audio_buffer_size  = 0;
//        static unsigned int audio_buffer_index = 0;
//
//        while (length > 0) {
//            if (!VP::isRunning())
//                return;
//            if (audio_buffer_index >= audio_buffer_size) {
//
//            }
//        }
//    }
//
//    int audio_decode_frame(CodecContext &codec_context, uint8_t *audio_buffer, const int buffer_size)
//    {
//        VP::Packet packet;
//        static uint8_t  *audio_packet_data = nullptr;
//        static AVFrame  *av_frame          = nullptr;
//        static int       audio_packet_size = 0;
//
//        av_frame = av_frame_alloc();
//        if (av_frame == nullptr) {
//            printf("Can't create avframe!\n");
//            return -1;
//        }
//        int length_1  = 0;
//        int data_size = 0;
//        
//        while(true) {
//            if (!VP::isRunning())
//                return -1;
//
//            while (audio_packet_size > 0) {
//                int got_frame = 0;
//                int ret = avcodec_receive_frame(codec_context.getAVCodecContext(), av_frame);
//                if (ret == 0)
//                {
//                    got_frame = 1;
//                }
//                if (ret == AVERROR(EAGAIN))
//                {
//                    ret = 0;
//                }
//                if (ret == 0)
//                {
//                    ret = avcodec_send_packet(codec_context.getAVCodecContext(), packet.getAVPacket());
//                }
//                if (ret == AVERROR(EAGAIN))
//                {
//                    ret = 0;
//                }
//                else if (ret < 0)
//                {
//                    printf("avcodec_receive_frame error");
//                    return -1;
//                }
//                else
//                {
//                    length_1 = packet.getSize();
//                }
//
//                if (length_1 < 0)
//                {
//                    // if error, skip frame
//                    audio_packet_size = 0;
//                    break;
//                }
//                audio_packet_data += length_1;
//                audio_packet_size -= length_1;
//                data_size          = 0;
//                if (got_frame) {
//                    data_size = audio_resampling();
//                    assert(data_size <= buffer_size);
//                }
//                if (data_size <= 0)
//                    continue;
//                return data_size;
//            }
//            if (packet.getAVPacket()->data)
//                packet.unref();
//            int ret = packet_queue.getAndPop(packet, true); 
//            if (ret < 0)
//                return -1;
//            audio_packet_data = packet.getAVPacket()->data;
//            audio_packet_size = packet.getSize();
//        }
//        return 0;
//    }
//
//    int audio_resampling(CodecContext &audio_decode_ctx, AVFrame *decoded_audio_frame, enum AVSampleFormat out_sample_fmt, int out_channels, int out_sample_rate, uint8_t * out_buf)
//    {
//        VP::SwrContext swr_context(audio_decode_ctx, AV_SAMPLE_FMT_S16, audio_decode_ctx.sampleRate());
//        int linesize = 0;
//        const int delay       = swr_get_delay(swr_context.getAVSwrContext(), decoded_audio_frame->sample_rate); 
//        int dst_samples       = decoded_audio_frame->ch_layout.nb_channels;
//        dst_samples          *= av_rescale_rnd(delay, 44100, decoded_audio_frame->sample_rate, AV_ROUND_UP);
//        uint8_t*    buffer    = nullptr;
//        int result            = av_samples_alloc(&buffer, nullptr, 1, dst_samples, AV_SAMPLE_FMT_S16, 1);
//        
//        dst_samples = decoded_audio_frame->ch_layout.nb_channels  * swr_context.convert(&buffer, dst_samples, (const uint8_t**)decoded_audio_frame->data, decoded_audio_frame->nb_samples);
//        int res_size = av_samples_get_buffer_size(&linesize, decoded_audio_frame->ch_layout.nb_channels, dst_samples, AV_SAMPLE_FMT_S16, 1);
//        memcpy(out_buf, &buffer[0], res_size);
//        av_freep(&buffer[0]);
//        av_free(&buffer);
//    }
}
