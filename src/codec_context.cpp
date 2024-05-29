#include "codec_context.hpp"
#include "codec.hpp"
#include "error.hpp"
#include "frame.hpp"
#include "packet.hpp"
#include <cassert>
#include <stdexcept>

using VP::CodecContext;

CodecContext::CodecContext(const Codec &codec, AVCodecParameters *parameters)
{
    const char *err_msg = "Can't allocate codec context!\n";
    m_codec_ctx = avcodec_alloc_context3(codec.getAVCodec());
    if (m_codec_ctx == nullptr) 
        throw std::runtime_error(err_msg);

    int result = avcodec_parameters_to_context(m_codec_ctx, parameters);
    check_libav_return_value(result);                                                                   

    result = avcodec_open2(m_codec_ctx, codec.getAVCodec(), nullptr);
    check_libav_return_value(result);                                                                   
}

CodecContext::~CodecContext()
{
    avcodec_free_context(&m_codec_ctx);     
}

CodecContext& CodecContext::operator=(CodecContext &&rhs) noexcept 
{ 
    if (this != &rhs) {
        if (m_codec_ctx != nullptr)
            avcodec_free_context(&m_codec_ctx); 
        m_codec_ctx     = rhs.m_codec_ctx; 
        rhs.m_codec_ctx = nullptr;
    }
    return *this;
}

void CodecContext::sendPacket(const Packet &packet)
{
    const int result = avcodec_send_packet(m_codec_ctx, &packet.getAVPacket());
    check_libav_return_value(result);
}

void CodecContext::flushBuffers() 
{
    avcodec_flush_buffers(m_codec_ctx);    
}

int CodecContext::receiveFrame(Frame &frame)
{
    return avcodec_receive_frame(m_codec_ctx, frame.getAVFrame());
}
 
[[nodiscard]] int64_t CodecContext::guess_correct_pts(const int64_t reordered_pts, int64_t dts) const noexcept
{
    int64_t pts = AV_NOPTS_VALUE;

    if (dts != AV_NOPTS_VALUE) {
        m_codec_ctx->pts_correction_num_faulty_dts += dts <= m_codec_ctx->pts_correction_last_dts;
        m_codec_ctx->pts_correction_last_dts = dts;
    } else if (reordered_pts != AV_NOPTS_VALUE) {
        m_codec_ctx->pts_correction_last_dts = reordered_pts;
    }

    if (reordered_pts != AV_NOPTS_VALUE) {
        m_codec_ctx->pts_correction_num_faulty_pts += reordered_pts <= m_codec_ctx->pts_correction_last_pts;
        m_codec_ctx->pts_correction_last_pts = reordered_pts;
    } else if (dts != AV_NOPTS_VALUE) {
        m_codec_ctx->pts_correction_last_pts = dts;
    }

    if ((m_codec_ctx->pts_correction_num_faulty_pts <= m_codec_ctx->pts_correction_num_faulty_dts || dts == AV_NOPTS_VALUE) && reordered_pts != AV_NOPTS_VALUE) {
        pts = reordered_pts;
    } else {
        pts = dts;
    }

    return pts;
}
