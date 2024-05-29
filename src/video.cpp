#include "video.hpp"
#include "packet.hpp"
#include "renderer.hpp"
#include "format_context.hpp"
#include "vp.hpp"
#include <cassert>
#include <cerrno>
#include <SDL_events.h>
#include <SDL_timer.h>

extern "C" {
    #include <libavformat/avformat.h>
    #include <libavutil/time.h>
}

using VP::Video;

Video::Video(const char *path_to_file, Window &window)
    : m_format_ctx(path_to_file),
    m_render(window),
    m_video_codec(m_format_ctx),
    m_frame(m_render, m_video_codec.width(), m_video_codec.height()),
    m_dest_frame(m_render, m_video_codec.width(), m_video_codec.height()),
    m_sws_context(m_video_codec, m_video_codec.width(), m_video_codec.height(), AV_PIX_FMT_YUV420P, SWS_BILINEAR),
    m_audio_frame_handler(m_format_ctx),
    m_video_queue(),
    m_is_playing(true),
    m_is_rendered(false),
    m_is_updated(false),
    m_timer(),
    m_frame_timer(),
    m_frame_last_pts(),
    m_frame_last_delay(),
    m_video_clock()
{
    m_timer = SDL_AddTimer(1, refreshTimer, this);
    check_sdl_return_value(m_timer);
}

Video::~Video()
{
    SDL_RemoveTimer(m_timer);    
    while (!m_video_queue.empty()) {
        m_video_queue.front().unref();
        m_video_queue.pop();
    }
}

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
        {
            m_video_queue.push(packet);
        }
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
            running      = false;
            m_is_playing = false;
            break;
        }

        if (m_is_rendered == false) {
            m_render.clear();
            m_render.copy(m_dest_frame.getTexture());
            m_render.present();
            m_is_rendered = true; }
        if (m_is_updated == true) {
            m_is_rendered = false;
            m_is_updated  = false;
        }
    }
}

Uint32 Video::refreshTimer(Uint32 interval, void *data)
{
    auto video = static_cast<Video*>(data);
    if (video->m_video_queue.empty() == true) {
        return 100;
    } else {
        Uint32 time= 0;
        auto &packet = video->m_video_queue.front();
        video->m_video_codec.sendPacket(packet);
        while (video->m_video_codec.receiveFrame(video->m_frame) >= 0) {
            double video_pts       = video->m_video_codec.guess_correct_pts(video->m_frame.getAVFrame()->pts, video->m_frame.getAVFrame()->pkt_dts);
            double video_pts_delay = 0.0;
            double audio_clock = 0.0;
            double video_audio_delay = 0.0;
            
            if (video_pts == AV_NOPTS_VALUE)
                video_pts = 0.0;

            video_pts      *= av_q2d(video->m_format_ctx.getVideoTimebase());
            video_pts       = video->synchronizeVideo(video->m_frame, video_pts);
            video_pts_delay = video_pts - video->m_frame_last_pts;
            time            = video_pts_delay * 1000;
            printf("PTS Delay:\t\t\t\t%f\n", video_pts_delay);
            printf("PTS Timer:\t\t\t\t%d\n", time);
            video->m_frame_last_delay = video_pts_delay;
            video->m_frame_last_pts   = video_pts;
            video->m_sws_context.scale(video->m_video_codec, video->m_frame, video->m_dest_frame);
        }
        video->m_is_updated = true;
        packet.unref();
        video->m_video_queue.pop();
        if (time == 0.0)
            return 10;
        else
            return time;
    }
}

double Video::synchronizeVideo(Frame &frame, double pts)
{
    double frame_delay = 0.0;

    if (pts != 0){
        // if we have pts, set video clock to it
        m_video_clock = pts;
    } else {
        // if we aren't given a pts, set it to the clock
        pts = m_video_clock;
    }

    // update the video clock
    frame_delay = av_q2d(m_video_codec.getAVCodecContext()->time_base);
    
    // if we are repeating a frame, adjust clock accordingly
    frame_delay += frame.getAVFrame()->repeat_pict * (frame_delay * 0.5);

    m_video_clock += frame_delay;

    return pts;
}
