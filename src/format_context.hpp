#pragma once
#include "def.hpp"

struct AVFormatContext;
struct AVStream;
struct AVCodecParameters;
struct AVPacket;

namespace VP {
    class FormatContext {
    public:
        enum class VideoFrameReturnValue {
            END_OF_STREAM,
            VIDEO_STREAM,
            OTHER_STREAM,
            ERROR
        };
        FormatContext(const char *path);
        ~FormatContext();
                
        AVFormatContext* ctx() const noexcept
        { return m_format_ctx; }

        void dumpFormat() const;

        unsigned int nb_streams() const noexcept;
        AVStream** streams() const noexcept;
        
        Codec getVideoCodec() const noexcept; 
        AVCodecParameters* videoStreamCodecParams() const noexcept;
        VideoFrameReturnValue getVideoFrame(AVPacket *packet) noexcept;
    private:
        AVFormatContext *m_format_ctx   = nullptr;
        const char      *m_path         = nullptr;
        int              m_video_stream = -1;
    };
}
