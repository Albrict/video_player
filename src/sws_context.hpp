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
        { *this = std::move(other); }
        ~SwsContext();

        SwsContext &operator=(SwsContext &&rhs) noexcept;

        SwsContext(const SwsContext &other) = delete;
        SwsContext &operator=(const SwsContext &rhs) = delete;

        void scale(const CodecContext &video_codec_context, const Frame &source_frame, Frame &dest_frame);

        [[nodiscard]] const ::SwsContext *getAVSwsContext() const noexcept
        {  return m_sws_context; }
        [[nodiscard]] ::SwsContext *getAVSwsContext() noexcept
        {  return m_sws_context; }
    private:
        ::SwsContext *m_sws_context {};
    };
}
