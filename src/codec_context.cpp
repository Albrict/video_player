#include "codec_context.hpp"
#include "codec.hpp"
#include "error.hpp"
#include "frame.hpp"
#include <stdexcept>

using namespace VP;

CodecContext::CodecContext(Codec codec, AVCodecParameters *parameters)
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


void CodecContext::sendPacket(AVPacket *packet)
{
    avcodec_send_packet(m_codec_ctx, packet);
}

void CodecContext::receiveFrame(Frame &frame)
{
    avcodec_receive_frame(m_codec_ctx, frame.getAVFrame());
}
