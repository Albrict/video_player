#include <SDL_mutex.h>
#include "picture_queue.hpp"
#include "vp.hpp"

using VP::PictureQueue;

PictureQueue::PictureQueue()
{
    m_mutex = SDL_CreateMutex();
    if (m_mutex == nullptr)
        throw std::runtime_error(SDL_GetError());
    m_cond  = SDL_CreateCond();
    if (m_cond == nullptr)
        throw std::runtime_error(SDL_GetError());
}


PictureQueue::Result PictureQueue::putFrameInQueue(Frame &frame, const double pts)
{
    SDL_LockMutex(m_mutex);
    while (m_picture_queue.size() >= video_picture_queue_size && VP::isRunning())
        SDL_CondWait(m_cond, m_mutex);
    SDL_UnlockMutex(m_mutex);
    if (VP::isRunning() == false)
        return Result::PROGRAM_IS_DONE;        
}
