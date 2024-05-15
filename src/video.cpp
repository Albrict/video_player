#include "video.hpp"
#include "renderer.hpp"
#include "format_context.hpp"
#include "vp.hpp"
#include <cassert>
#include <cerrno>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>

extern "C" {
    #include <libavformat/avformat.h>
}

using VP::Video;
using VP::FormatContext;

Video::Video(const char *path_to_file, Renderer &render)
    : m_format_ctx(path_to_file),
    m_video_frame_handler(render, m_format_ctx),
    m_audio_frame_handler(m_format_ctx),
    m_packet()
{}

[[nodiscard]] FormatContext::FrameType Video::readFrame()
{
    const auto type = m_format_ctx.readFrame(m_packet);
    if (type == FormatContext::FrameType::OTHER_STREAM)
        m_packet.unref();
    return type;
}

void Video::renderFrame(Renderer &render)
{
    m_video_frame_handler.renderFrame(render, m_packet);
    m_packet.unref();
}

void Video::playFrame()
{
    m_audio_frame_handler.playFrame(m_packet);
    m_packet.unref();
}
