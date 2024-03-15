#include <stdexcept>
extern "C" {
    #include <libavutil/error.h>
    #include <libavutil/avutil.h>
    #include <libavcodec/avcodec.h>
    #include <libavcodec/codec.h>
    #include <libavutil/imgutils.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
    #include <libswscale/swscale.h>
}

#include "vp_error.hpp"
#include "video.hpp"

VP::Video::Video(const char *path_to_file, SDL_Renderer *render)
{
    // FFMPEG init
    const char *format_ctx_alloc_error  = "avformat_alloc_context error!\n";
    const char *video_stream_error      = "Can't find video stream\n";
    const char *unsupported_codec_error = "Unsupported codec!\n";
    const char *codec_ctx_alloc_error   = "avcodec_alloc_context3 error!\n";
    const char *sws_ctx_error           = "Can't get sws context\n"; 
    const char *pixel_buffer_error      = "Could not allocate pixel buffers - exiting\n";
    const char *frame_allocation_error  = "av_frame_alloc error!\n";
    m_frame      = VP::check_callnullptr<AVFrame*>(frame_allocation_error, av_frame_alloc);
    m_format_ctx = VP::check_callnullptr<AVFormatContext*>(format_ctx_alloc_error, avformat_alloc_context);
    
    VP::check_call(avformat_open_input, &m_format_ctx, path_to_file, nullptr, nullptr); // Open video file
    VP::check_call(avformat_find_stream_info, m_format_ctx, nullptr); // Retrieve stream information
    av_dump_format(m_format_ctx, 0, path_to_file, 0); // Dump information about file onto standard error
    
    // Find the first video stream
    for (int i = 0; i < m_format_ctx->nb_streams; i++){
        if (m_format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            m_video_stream = i;
            break;
        }
    }
    // Didn't find a video stream
    if (m_video_stream == -1)
        throw std::runtime_error(video_stream_error);
        
    AVCodecParameters *params = m_format_ctx->streams[m_video_stream]->codecpar;  // Get a pointer to the codec parameters 
    // Find the decoder for the video stream
    m_codec     = VP::check_callnullptr<const AVCodec*>(unsupported_codec_error, avcodec_find_decoder, params->codec_id);
    m_codec_ctx = VP::check_callnullptr<AVCodecContext*>(codec_ctx_alloc_error, avcodec_alloc_context3, m_codec); // allocate codec_ctx 
    
    VP::check_call(avcodec_parameters_to_context, m_codec_ctx, params); // copy params to context
    VP::check_call(avcodec_open2, m_codec_ctx, m_codec, nullptr);  // Open codec
    // initialize SWS context for software scaling
    m_sws_ctx = VP::check_callnullptr<SwsContext*>(sws_ctx_error, sws_getContext, 
                                                    m_codec_ctx->width, m_codec_ctx->height,
                                                    m_codec_ctx->pix_fmt, m_codec_ctx->width, m_codec_ctx->height,
                                                    AV_PIX_FMT_YUV420P, SWS_BILINEAR, nullptr, nullptr, nullptr); 
    
    // set up YV12 pixel array (12 bits per pixel)
    size_t y_plane_sz  = m_codec_ctx->width * m_codec_ctx->height;
    size_t uv_plane_sz = m_codec_ctx->width * m_codec_ctx->height / 4;
    m_uv_pitch     = m_codec_ctx->width / 2;

    m_y_plane = static_cast<Uint8*>(av_malloc(y_plane_sz));
    m_u_plane = static_cast<Uint8*>(av_malloc(uv_plane_sz));
    m_v_plane = static_cast<Uint8*>(av_malloc(uv_plane_sz));
    
    if (!m_y_plane || !m_u_plane || !m_v_plane) 
        throw std::runtime_error(pixel_buffer_error);
    
    // SDL init 
    m_texture = VP::check_SDLnullptr<SDL_Texture*>(SDL_CreateTexture, render, 
                                                SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, 
                                                m_codec_ctx->width, m_codec_ctx->height); 
}

VP::Video::~Video()
{
    av_frame_free(&m_frame);
    avcodec_free_context(&m_codec_ctx); 
    avformat_free_context(m_format_ctx);
    sws_freeContext(m_sws_ctx);
    av_free(m_y_plane);
    av_free(m_u_plane);
    av_free(m_v_plane);

    SDL_DestroyTexture(m_texture);
    
}

SDL_Texture *VP::Video::getFrame()
{
    const int result = av_read_frame(m_format_ctx, &m_packet);
    if (result < 0) {
        return nullptr;
    } else {
        if (m_packet.stream_index == m_video_stream) {
            avcodec_send_packet(m_codec_ctx, &m_packet);
            avcodec_receive_frame(m_codec_ctx, m_frame);

            AVFrame *pict = av_frame_alloc();
            pict->data[0] = m_y_plane;
            pict->data[1] = m_u_plane;
            pict->data[2] = m_v_plane;
            pict->linesize[0] = m_codec_ctx->width;
            pict->linesize[1] = m_uv_pitch;
            pict->linesize[2] = m_uv_pitch;
            
            sws_scale(m_sws_ctx, (uint8_t const * const *) m_frame->data,
                    m_frame->linesize, 0, m_codec_ctx->height, pict->data,
                    pict->linesize);

            SDL_UpdateYUVTexture(
                    m_texture,
                    NULL,
                    m_y_plane,
                    m_codec_ctx->width,
                    m_u_plane,
                    m_uv_pitch,
                    m_v_plane,
                    m_uv_pitch
                );
            av_frame_free(&pict);
        }
        av_packet_unref(&m_packet);
        return m_texture;
    }
}
