#include "sws_context.hpp"
#include "frame.hpp"
#include "codec_context.hpp"
#include <stdexcept>

extern "C" {
    #include <libswscale/swscale.h>
}

using VP::Frame;

VP::SwsContext::SwsContext(CodecContext &codec_context, const int dst_width, const int dst_height, 
            const AVPixelFormat pixel_format, const int rescaling_flags_and_options)
{
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

void VP::SwsContext::scale(const VP::CodecContext &video_codec_context, const Frame &source_frame, Frame &dest_frame)
{
    const AVFrame *source_avframe = source_frame.getAVFrame();
    AVFrame       *dest_avframe   = dest_frame.getAVFrame();

    if (source_avframe->data[0]) {
        sws_scale(m_sws_context, source_avframe->data, source_avframe->linesize, 0, video_codec_context.height(), dest_avframe->data, dest_avframe->linesize);
        dest_frame.updateYUV(video_codec_context);
    }
}
