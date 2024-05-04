#pragma once
#include <SDL_stdinc.h>
#include <memory>
#include <vector>
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
    class Frame {
    public:
        struct YuvData {
            std::vector<Uint8> m_y_plane;
            std::vector<Uint8> m_u_plane;
            std::vector<Uint8> m_v_plane;
            int                m_uv_pitch;

            explicit YuvData(const size_t y_plane_size, const size_t uv_plane_size, const int uv_pitch)
                : m_y_plane(y_plane_size),
                m_u_plane(uv_plane_size),
                m_v_plane(uv_plane_size),
                m_uv_pitch(uv_pitch) {}
            ~YuvData() = default;
        };

        explicit Frame(Renderer &render, const int texture_width, const int texture_height);
        ~Frame();
    
        void updateYUV(const CodecContext &video_codec_context);
        void scaleInto(Frame &dest, SwsContext &context, const int width, const int height);
        [[nodiscard]] AVFrame *getAVFrame() const noexcept
        { return m_frame; }
        [[nodiscard]] Texture *getTexture() const noexcept
        { return m_texture.get(); }
    private: 
        std::unique_ptr<Texture> m_texture  {};
        std::unique_ptr<YuvData> m_yuv_data {};
        AVFrame                 *m_frame    {}; 
    };
}
