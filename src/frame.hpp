#pragma once
#include <SDL_stdinc.h>
#include <stdexcept>
#include "def.hpp"

extern "C" {
    #include <libavutil/mem.h>
    #include <libavutil/frame.h>
}

namespace VP {
    class Frame {
    public:
        explicit Frame()
        {
            const char *frame_allocation_error  = "av_frame_alloc error!\n";
            m_frame = av_frame_alloc();
            if (!m_frame)
                throw std::runtime_error(frame_allocation_error);
        }
        Frame(Frame &&other) noexcept 
        { *this = std::move(other); }

        virtual ~Frame() 
        { av_frame_free(&m_frame); }

        Frame &operator=(Frame &&rhs) noexcept 
        {
            if (this != &rhs) {
                m_frame   = rhs.m_frame;
                rhs.m_frame = nullptr;
            }
            return *this;
        }

        [[nodiscard]] virtual AVFrame *getAVFrame() const noexcept final 
        { return m_frame; }
        
        [[nodiscard]] virtual uint8_t **getFrameData() const noexcept final 
        { return m_frame->data; }

        [[nodiscard]] virtual int64_t getPts() const noexcept final 
        { return m_frame->pts; }
        [[nodiscard]] virtual int64_t getDts() const noexcept final
        { return m_frame->pkt_dts; }

    protected: 
        AVFrame *m_frame {}; 
    };
}
