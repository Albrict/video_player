#pragma once
#include "def.hpp"
#include <utility>

struct AVFormatContext;
struct AVStream;
struct AVCodecParameters;
struct AVPacket;

extern "C" {
    #include <libavutil/rational.h>
}

namespace VP {
    class FormatContext final {
    public:
        enum class FrameType {
            END_OF_STREAM,
            VIDEO_STREAM,
            AUDIO_STREAM,
            OTHER_STREAM,
            ERROR
        };
        explicit FormatContext(const char *path);
        FormatContext(FormatContext &&other) noexcept
        { *this = std::move(other); }

        ~FormatContext();

        FormatContext &operator=(FormatContext &&rhs) noexcept;
        [[nodiscard]] AVFormatContext* ctx() const noexcept
        { return m_format_ctx; }

        [[nodiscard]] unsigned int nb_streams() const noexcept;
        [[nodiscard]] AVStream** streams() const noexcept;
        
        [[nodiscard]] Codec getVideoCodec() const; 
        [[nodiscard]] Codec getAudioCodec() const;
        [[nodiscard]] AVCodecParameters* videoStreamCodecParams() const noexcept;
        [[nodiscard]] AVCodecParameters* audioStreamCodecParams() const noexcept;
        [[nodiscard]] FrameType readFrame(Packet &packet) noexcept;
        [[nodiscard]] AVRational getAverageFrameRate() const noexcept; 
        [[nodiscard]] AVRational getVideoTimebase() const noexcept;  
        [[nodiscard]] AVRational getAudioTimebase() const noexcept;

        FormatContext(const FormatContext &other) = delete;
        FormatContext &operator=(const FormatContext &rhs) = delete;
    private:
        AVFormatContext *m_format_ctx   {};
        int              m_video_stream {-1};
        int              m_audio_stream {-1};
    };
}
