#include "video.hpp"
#include "format_context.hpp"
#include "frame.hpp"
#include "codec_context.hpp"
#include "codec.hpp"

extern "C" {
    #include <libavformat/avformat.h>
}

using namespace VP;

Video::Video(const char *path_to_file, Renderer &render)
    : m_format_ctx(path_to_file),
    m_video_codec_context(m_format_ctx.getVideoCodec(), m_format_ctx.videoStreamCodecParams()),
    m_sws_context(m_video_codec_context, m_video_codec_context.width(), m_video_codec_context.height(), AV_PIX_FMT_YUV420P, SWS_BILINEAR),
    m_frame(render, m_video_codec_context.width(), m_video_codec_context.height()),
    m_dest_frame(render, m_video_codec_context.width(), m_video_codec_context.height())
{
}

Texture *Video::getFrame()
{
    const auto result = m_format_ctx.getVideoFrame(&m_packet);
    switch(result) {
    using enum FormatContext::VideoFrameReturnValue;
    case END_OF_STREAM:
        return nullptr;
    case VIDEO_STREAM:
        m_video_codec_context.sendPacket(&m_packet);
        m_video_codec_context.receiveFrame(m_frame);
        m_sws_context.scale(m_video_codec_context, m_frame, m_dest_frame);
        av_packet_unref(&m_packet);
        return m_dest_frame.getTexture();
    case ERROR:
        return nullptr;
    case OTHER_STREAM:
        av_packet_unref(&m_packet);
        return m_dest_frame.getTexture();
        break;
    };
}
