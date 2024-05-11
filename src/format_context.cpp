#include "format_context.hpp"
#include "codec.hpp"
#include "error.hpp"
#include "packet.hpp"
#include <cassert>

extern "C" {
    #include <libavformat/avformat.h>
}

using namespace VP;

FormatContext::FormatContext(const char *path)
{
    assert(path != nullptr && "Path is null!");
    int result = avformat_open_input(&m_format_ctx, path, nullptr, nullptr); // Open video file
    check_libav_return_value(result);

    result = avformat_find_stream_info(m_format_ctx, nullptr);               // Retrieve stream information
    check_libav_return_value(result);

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

FormatContext::VideoFrameReturnValue FormatContext::getVideoFrame(Packet &packet) noexcept
{
    int result = av_read_frame(m_format_ctx, packet.getAVPacket());
    if (result < 0) {
        if (packet.isEmpty())
            return VideoFrameReturnValue::ERROR;
        else
            return VideoFrameReturnValue::END_OF_STREAM;
    } else {
        if (packet.getStreamIndex() == m_video_stream)
            return VideoFrameReturnValue::VIDEO_STREAM; 
        else
            return VideoFrameReturnValue::OTHER_STREAM;
    }
}

FormatContext& FormatContext::operator=(FormatContext &&rhs) noexcept
{
    if (this != &rhs) {
        if (m_format_ctx != nullptr)
            avformat_close_input(&m_format_ctx);
        m_format_ctx         = rhs.m_format_ctx;
        m_video_stream       = rhs.m_video_stream;
        rhs.m_format_ctx     = nullptr;
        rhs.m_video_stream   = -1;
    }
    return *this;
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
