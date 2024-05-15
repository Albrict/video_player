#pragma once
#include "frame.hpp"
#include "yuv_data.hpp"
#include "texture.hpp"

namespace VP {
    class VideoFrame final : public Frame {
    public:
        explicit VideoFrame(Renderer &render, const int width, const int height)
            : Frame(),
            m_yuv_data(width * height, width * height / 4, width * height / 4, width, width / 2, width / 2),
            texture(render, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, width, height) 
        {
            m_frame->data[0]     = m_yuv_data.m_y_plane.data();
            m_frame->data[1]     = m_yuv_data.m_u_plane.data();
            m_frame->data[2]     = m_yuv_data.m_v_plane.data();
            m_frame->linesize[0] = width;
            m_frame->linesize[1] = m_yuv_data.m_u_pitch;
            m_frame->linesize[2] = m_yuv_data.m_v_pitch;
        }
        ~VideoFrame() = default;

        [[nodiscard]] const Texture &getTexture() const noexcept
        { return texture; }

        [[nodiscard]] Texture &getTexture() noexcept
        { return texture; }

        void updateYUV(SDL_Rect *rect = nullptr)
        { texture.updateYUV(rect, m_yuv_data); }
    private:
        YuvData m_yuv_data;
        Texture texture;
    };
}
