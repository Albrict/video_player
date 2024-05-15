#include "video_frame_handler.hpp"
#include "packet.hpp"
#include "renderer.hpp"

extern "C" {
    #include <libswscale/swscale.h>
}

using VP::VideoFrameHandler;


VideoFrameHandler::VideoFrameHandler(Renderer &render, FormatContext &format_context)
    : m_video_codec(format_context),
    m_frame(render, m_video_codec.width(), m_video_codec.height()),
    m_dest_frame(render, m_video_codec.width(), m_video_codec.height()),
    m_sws_context(m_video_codec, m_video_codec.width(), m_video_codec.height(), AV_PIX_FMT_YUV420P, SWS_BILINEAR)
{}

void VideoFrameHandler::renderFrame(Renderer &render, Packet &packet)
{
    m_video_codec.sendPacket(packet);
    while (m_video_codec.receiveFrame(m_frame) >= 0) {
        m_sws_context.scale(m_video_codec, m_frame, m_dest_frame);
        render.copy(m_dest_frame.getTexture());    
    }
}
