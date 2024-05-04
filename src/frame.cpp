#include <stdexcept>
#include "frame.hpp"
#include "renderer.hpp"
#include "texture.hpp"
#include "codec_context.hpp"

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libswscale/swscale.h>
}

using namespace VP;

Frame::Frame(Renderer &render, const int texture_width, const int texture_height)
{
    const char *frame_allocation_error  = "av_frame_alloc error!\n";

    m_frame = av_frame_alloc();
    if (!m_frame)
        throw std::runtime_error(frame_allocation_error);
    m_texture  = std::make_unique<Texture>(render, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, texture_width, texture_height);

    const size_t      y_plane_size            = texture_width * texture_height;
    const size_t      uv_plane_size           = texture_width * texture_height / 4;
    const size_t      uv_pitch                = texture_width / 2;

    m_yuv_data = std::make_unique<YuvData>(y_plane_size, uv_plane_size, uv_pitch);
    m_frame->data[0]     = m_yuv_data->m_y_plane.data();
    m_frame->data[1]     = m_yuv_data->m_u_plane.data();
    m_frame->data[2]     = m_yuv_data->m_v_plane.data();
    m_frame->linesize[0] = texture_width;
    m_frame->linesize[1] = m_yuv_data->m_uv_pitch;
    m_frame->linesize[2] = m_yuv_data->m_uv_pitch;
}

void Frame::updateYUV(const CodecContext &video_codec_context)
{
    m_texture->updateYUV(nullptr, m_yuv_data->m_y_plane.data(), video_codec_context.width(), 
            m_yuv_data->m_u_plane.data(), m_yuv_data->m_uv_pitch, m_yuv_data->m_v_plane.data(), m_yuv_data->m_uv_pitch); 
}

Frame::~Frame()
{
    av_frame_free(&m_frame);
}
