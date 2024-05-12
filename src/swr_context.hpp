#pragma once
#include "error.hpp"
#include "frame.hpp"

extern "C" {
    #include <libswresample/swresample.h>
}

struct SwrContext;
namespace VP {
    class SwrContext final {
    public:
        SwrContext(const CodecContext &codec_context, const AVSampleFormat format, 
                   const int out_sample_rate, const int log_offset = 0, void *log_ctx = nullptr);
        ~SwrContext();
        
        [[nodiscard]] int getDelay(const Frame &frame) const noexcept
        { return swr_get_delay(m_swr_context, frame.getSampleRate()); }

        int convert(uint8_t **out, const int out_count, const uint8_t **in, const int in_count) const noexcept
        { 
            const int result = swr_convert(m_swr_context, out, out_count, in, in_count); 
            check_libav_return_value(result);
            return result;
        }

        const ::SwrContext *getAVSwrContext() const noexcept
        { return m_swr_context; }

        ::SwrContext *getAVSwrContext() noexcept
        { return m_swr_context; }
    private:
        ::SwrContext *m_swr_context {};
    };
}
