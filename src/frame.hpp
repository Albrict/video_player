#pragma once
#include <SDL_stdinc.h>
#include <memory>
#include "yuv_data.hpp"
#include "def.hpp"

extern "C" {
    #include <libavutil/mem.h>
}

struct AVFrame;
struct AVCodecContext;
struct AVPacket;
struct SwsContext;
struct SDL_Rect;

namespace VP {
    class Frame final {
    public:

        explicit Frame(Renderer &render, const int texture_width, const int texture_height);
        Frame(Renderer &render, const Frame &frame);
        Frame(Frame &&frame) noexcept;
        ~Frame();
    
        void updateYUV(const CodecContext &video_codec_context);
        void scaleInto(Frame &dest, SwsContext &context, const int width, const int height);
        [[nodiscard]] AVFrame *getAVFrame() const noexcept
        { return m_frame; }
        [[nodiscard]] Texture *getTexture() const noexcept
        { return m_texture.get(); }

        Frame &operator=(Frame &&frame) noexcept
        { return *this = Frame(std::move(frame)); }

    private: 
        YuvData                  m_yuv_data;
        std::unique_ptr<Texture> m_texture  {};
        AVFrame                 *m_frame    {}; 
    };
}
