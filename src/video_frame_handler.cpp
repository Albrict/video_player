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
    m_sws_context(m_video_codec, m_video_codec.width(), m_video_codec.height(), AV_PIX_FMT_YUV420P, SWS_BILINEAR),
    m_packet_queue(),
    m_thread(nullptr, nullptr, nullptr),
    m_video_clock(),
    m_timebase(format_context.getVideoTimebase())
{
      
}

void VideoFrameHandler::videoThread(void *data)
{
    VideoFrameHandler *video_handler = static_cast<VideoFrameHandler*>(data);
    Packet packet;
    double pts;
    bool   frame_finished = false;
    for (;;) {
        const PacketQueue::Result result = video_handler->m_packet_queue.getPacket(packet, true);
        if (result == PacketQueue::Result::PROGRAM_IS_DONE)
            break;
        pts = 0.0;
        video_handler->m_video_codec.sendPacket(packet);
        const int ret = video_handler->m_video_codec.receiveFrame(video_handler->m_frame); 
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            break;
        else
            frame_finished = true;
        pts = video_handler->m_video_codec.guess_correct_pts(video_handler->m_frame.getPts(), video_handler->m_frame.getDts());
        if (pts == AV_NOPTS_VALUE)
            pts = 0.0;
        pts *= av_q2d(video_handler->m_timebase);
        if (frame_finished) {
            pts = video_handler->synchronizeVideo(pts);
        }
    }
}

double VideoFrameHandler::synchronizeVideo(double pts) noexcept
{
    double frame_delay;

    if (pts != 0) {
        m_video_clock.video_clock = pts; // if we have pts, set video clock to it
    } else {
        pts = m_video_clock.video_clock;   // if we aren't given a pts, set it to the clock
    }

    // update the video clock
    frame_delay = av_q2d(m_timebase);

    // if we are repeating a frame, adjust clock accordingly
    frame_delay += m_frame.getAVFrame()->repeat_pict * (frame_delay * 0.5);

    m_video_clock.video_clock += frame_delay;
    return pts;
}

void VideoFrameHandler::putPacketInDecodeQueue(Packet &packet)
{
    
}
