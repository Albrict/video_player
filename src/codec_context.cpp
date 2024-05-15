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
    const int result = avcodec_send_packet(m_codec_ctx, packet.getAVPacket());
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
