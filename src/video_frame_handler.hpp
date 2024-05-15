#pragma once
#include "def.hpp"
#include "video_frame.hpp"
#include "video_codec.hpp"
#include "sws_context.hpp"

namespace VP {
    class VideoFrameHandler {
    public:
        VideoFrameHandler(Renderer &render, FormatContext &format_context);

        void renderFrame(Renderer &render, Packet &packet);
    private:
        VideoCodec m_video_codec;
        VideoFrame m_frame;
        VideoFrame m_dest_frame;
        SwsContext m_sws_context;
    };
}
