#include "video.hpp"
#include "packet.hpp"
#include "renderer.hpp"
#include "format_context.hpp"
#include "vp.hpp"
#include <cassert>
#include <cerrno>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
#include <SDL_events.h>

extern "C" {
    #include <libavformat/avformat.h>
}

using VP::Video;

Video::Video(const char *path_to_file, Window &window)
    : m_format_ctx(path_to_file),
    m_render(window),
    m_video_frame_handler(m_render, m_format_ctx),
    m_audio_frame_handler(m_format_ctx),
    m_video_queue(),
    m_is_playing(true)
{}

void Video::play()
{
    Packet packet;
    bool running = true;
    while (running) {
        SDL_Event event {};
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
            running = false;

        const auto type = m_format_ctx.readFrame(packet);
        switch(type) {
        using enum FormatContext::FrameType;
        case VIDEO_STREAM:   
            m_video_queue.push(packet);
            packet.unref();
            break;
        case AUDIO_STREAM:
            m_audio_frame_handler.playFrame(packet);
            packet.unref();
            break;
        case ERROR:
            break;
        case OTHER_STREAM:
            packet.unref();
            break;
        case END_OF_STREAM:
            running = false;
            break;
        }
    }
}
