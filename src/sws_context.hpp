#pragma once
#include "def.hpp"
extern "C" {
    #include <libavutil/pixfmt.h>
}

struct SwsContext;

namespace VP {
    class SwsContext {
    public:
        SwsContext(CodecContext &codec_context, const int dst_width, const int dst_height, 
                    const AVPixelFormat pixel_format, const int rescaling_flags_and_options);
        ~SwsContext();

        void scale(const CodecContext &video_codec_context, const Frame &source_frame, Frame &dest_frame);
    private:
        ::SwsContext *m_sws_context;
    };
}
