#include "format_context.hpp"
#include "codec.hpp"
#include "error.hpp"

extern "C" {
    #include <libavformat/avformat.h>
}

using namespace VP;

FormatContext::FormatContext(const char *path)
{
    const char *format_ctx_alloc_error  = "avformat_alloc_context error!\n";
    VP::check_call(avformat_open_input, &m_format_ctx, path, nullptr, nullptr); // Open video file
    VP::check_call(avformat_find_stream_info, m_format_ctx, nullptr); // Retrieve stream information
    m_path = path;

    // Find the first video stream
    for (int i = 0; i < m_format_ctx->nb_streams; i++){
        if (m_format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            m_video_stream = i;
        }
    }
}

FormatContext::~FormatContext()
{
    avformat_close_input(&m_format_ctx);
}

FormatContext::VideoFrameReturnValue FormatContext::getVideoFrame(AVPacket *packet) noexcept
{
    int result = av_read_frame(m_format_ctx, packet);
    if (result < 0) {
        if (packet->data == nullptr)
            return VideoFrameReturnValue::ERROR;
        else
            return VideoFrameReturnValue::END_OF_STREAM;
    } else {
        if (packet->stream_index == m_video_stream)
            return VideoFrameReturnValue::VIDEO_STREAM; 
        else
            return VideoFrameReturnValue::OTHER_STREAM;
    }
}

unsigned int FormatContext::nb_streams() const noexcept
{
    return m_format_ctx->nb_streams; 
}

AVStream** FormatContext::streams() const noexcept
{
    return m_format_ctx->streams;
}

AVCodecParameters* FormatContext::videoStreamCodecParams() const noexcept
{
    return m_format_ctx->streams[m_video_stream]->codecpar;
}

Codec FormatContext::getVideoCodec() const noexcept
{
    AVCodecParameters *params = m_format_ctx->streams[m_video_stream]->codecpar;  // Get a pointer to the codec parameters 
    return Codec(params->codec_id);                                          // Find the decoder for the video stream
}

void FormatContext::dumpFormat() const 
{
    av_dump_format(m_format_ctx, 0, m_path, 0); // Dump information about file onto standard error
}