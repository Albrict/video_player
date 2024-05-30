#pragma once
#include "sws_context.hpp"
#include "thread.hpp"
#include "video_codec.hpp"
#include "video_frame.hpp"
#include "packet_queue.hpp"

namespace VP {
    class VideoFrameHandler final {
    public:
        explicit VideoFrameHandler(Renderer &render, FormatContext &format_context);
        ~VideoFrameHandler() = default;

        void renderFrame(Renderer &render, Packet &packet);
        void putPacketInDecodeQueue(Packet &packet);
    private:
        static void videoThread(void *data);
        [[nodiscard]] double synchronizeVideo(double pts) noexcept;
    private:
        struct VideoClock {
            double frame_timer      {};
            double frame_last_pts   {};
            double frame_last_delay {};
            double video_clock      {};
        };

        VideoCodec  m_video_codec;
        VideoFrame  m_frame;
        VideoFrame  m_dest_frame;
        SwsContext  m_sws_context;
        PacketQueue m_packet_queue;
        Thread      m_thread;
        VideoClock  m_video_clock;
        AVRational  m_timebase;
    };
}
