#pragma once
#include <SDL_stdinc.h>
#include <memory>
#include "yuv_data.hpp"
#include "def.hpp"

extern "C" {
    #include <libavutil/mem.h>
    #include <libavutil/frame.h>
}

struct AVCodecContext;
struct AVPacket;
struct SwsContext;
struct SDL_Rect;

namespace VP {
    class Frame final {
    public:

        explicit Frame(Renderer &render, const int texture_width, const int texture_height);
        Frame(Frame &&other) noexcept;

        ~Frame();
    
        void updateYUV(const CodecContext &video_codec_context);
        void scaleInto(Frame &dest, SwsContext &context, const int width, const int height);
        [[nodiscard]] AVFrame *getAVFrame() const noexcept
        { return m_frame; }
        [[nodiscard]] Texture *getTexture() const noexcept
        { return m_texture.get(); }
        
        [[nodiscard]] int getChannels() const noexcept 
        { return m_frame->ch_layout.nb_channels; }
        [[nodiscard]] int getSampleRate() const noexcept
        { return m_frame->sample_rate; }
        [[nodiscard]] int getNumberOfSamples() const noexcept
        { return m_frame->nb_samples; }
        [[nodiscard]] uint8_t **getFrameData() const noexcept
        { return m_frame->data; }
        
        Frame &operator=(Frame &&rhs) noexcept;
    private: 
        YuvData                  m_yuv_data;
        std::unique_ptr<Texture> m_texture  {};
        AVFrame                 *m_frame    {}; 
    };
}
