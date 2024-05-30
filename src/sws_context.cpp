#include "sws_context.hpp"
#include "video_frame.hpp"
#include "video_codec.hpp"
#include <cassert>
#include <stdexcept>

extern "C" {
    #include <libswscale/swscale.h>
}

using VP::Frame;

VP::SwsContext::SwsContext(VideoCodec &codec_context, const int dst_width, const int dst_height, 
            const AVPixelFormat pixel_format, const int rescaling_flags_and_options)
{
    assert(dst_width > 0 && dst_height > 0 && "Width or height are invalid!");

    const int   codec_width        = codec_context.width();
    const int   codec_height       = codec_context.height();
    const auto  codec_pixel_format = codec_context.pixelFormat(); 
    const char *error              = "Can't get Sws context!\n";
    m_sws_context = sws_getContext(codec_width, codec_height, codec_pixel_format, dst_width, dst_height, 
                                    pixel_format, rescaling_flags_and_options, nullptr, nullptr, nullptr); 
    if (m_sws_context == nullptr) 
        throw std::runtime_error(error);
}

VP::SwsContext::~SwsContext()
{
    sws_freeContext(m_sws_context);
}

VP::SwsContext& VP::SwsContext::operator=(SwsContext &&rhs) noexcept
{
    if (this != &rhs) {
        if (m_sws_context != nullptr)
            sws_freeContext(m_sws_context);
        m_sws_context     = rhs.m_sws_context;
        rhs.m_sws_context = nullptr;
    }
    return *this;
}

void VP::SwsContext::scale(const VP::VideoCodec &video_codec, const VideoFrame &source_frame, VideoFrame &dest_frame)
{
    const AVFrame *source_avframe = source_frame.getAVFrame();
    AVFrame       *dest_avframe   = dest_frame.getAVFrame();
    
    if (source_avframe->data[0]) {
        sws_scale(m_sws_context, source_avframe->data, source_avframe->linesize, 0, video_codec.height(), dest_avframe->data, dest_avframe->linesize);
        dest_frame.updateYUV();
    }
}
