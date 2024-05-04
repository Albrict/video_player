#pragma once
#include "def.hpp"
#include <utility>

struct AVFormatContext;
struct AVStream;
struct AVCodecParameters;
struct AVPacket;

namespace VP {
    class FormatContext final {
    public:
        enum class VideoFrameReturnValue {
            END_OF_STREAM,
            VIDEO_STREAM,
            OTHER_STREAM,
            ERROR
        };
        explicit FormatContext(const char *path);
        FormatContext(FormatContext &&other) noexcept
        {
            m_format_ctx         = other.m_format_ctx;
            m_video_stream       = other.m_video_stream;
            other.m_format_ctx   = nullptr;
            other.m_video_stream = -1;
        }
        ~FormatContext();

        FormatContext &operator=(FormatContext &&rhs) noexcept
        { return *this = FormatContext(std::move(rhs)); }

        [[nodiscard]] AVFormatContext* ctx() const noexcept
        { return m_format_ctx; }

        [[nodiscard]] unsigned int nb_streams() const noexcept;
        [[nodiscard]] AVStream** streams() const noexcept;
        
        [[nodiscard]] Codec getVideoCodec() const noexcept; 
        [[nodiscard]] AVCodecParameters* videoStreamCodecParams() const noexcept;
        [[nodiscard]] VideoFrameReturnValue getVideoFrame(AVPacket *packet) noexcept;


        FormatContext(const FormatContext &other) = delete;
        FormatContext &operator=(const FormatContext &rhs) = delete;
    private:
        AVFormatContext *m_format_ctx   {};
        int              m_video_stream {-1};
    };
}
