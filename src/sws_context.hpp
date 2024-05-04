#pragma once
#include "def.hpp"
#include <utility>
extern "C" {
    #include <libavutil/pixfmt.h>
}

struct SwsContext;

namespace VP {
    class SwsContext final {
    public:
        explicit SwsContext(CodecContext &codec_context, const int dst_width, const int dst_height, 
                    const AVPixelFormat pixel_format, const int rescaling_flags_and_options);
        SwsContext(SwsContext &&other) noexcept
        {
            m_sws_context = other.m_sws_context;
            other.m_sws_context = nullptr;
        }
        ~SwsContext();

        SwsContext &operator=(SwsContext &&rhs) noexcept
        { return *this = SwsContext(std::move(rhs)); }

        SwsContext(const SwsContext &other) = delete;
        SwsContext &operator=(const SwsContext &rhs) = delete;

        void scale(const CodecContext &video_codec_context, const Frame &source_frame, Frame &dest_frame);
    private:
        ::SwsContext *m_sws_context;
    };
}
