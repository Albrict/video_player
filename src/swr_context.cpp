#include "swr_context.hpp"
#include "codec_context.hpp"
#include "error.hpp"

VP::SwrContext::SwrContext(const CodecContext &codec_context, const AVSampleFormat format, 
           const int out_sample_rate, const int log_offset, void *log_ctx)
{
    auto channel_layout = codec_context.channelLayout();
    int result          = swr_alloc_set_opts2(&m_swr_context, &channel_layout, format, out_sample_rate, 
                                 &channel_layout, codec_context.sampleFormat(), codec_context.sampleRate(), log_offset, log_ctx);
    check_libav_return_value(result);  

    result = swr_init(m_swr_context);
    check_libav_return_value(result);  
}

VP::SwrContext::~SwrContext()
{
    swr_free(&m_swr_context);
}
