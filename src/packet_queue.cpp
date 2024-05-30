#include <SDL_mutex.h>
#include <stdexcept>
#include "packet_queue.hpp"
#include "vp.hpp"

using VP::PacketQueue;
using VP::Packet;

PacketQueue::PacketQueue()
{
    m_mutex = SDL_CreateMutex();
    if (m_mutex == nullptr)
        throw std::runtime_error(SDL_GetError());
    m_cond  = SDL_CreateCond();
    if (m_cond == nullptr)
        throw std::runtime_error(SDL_GetError());
}

PacketQueue::~PacketQueue()
{
    SDL_DestroyMutex(m_mutex);
    SDL_DestroyCond(m_cond);
}
    
void PacketQueue::putPacket(Packet &packet)
{
    SDL_LockMutex(m_mutex);
    m_queue.push(packet);
    SDL_CondSignal(m_cond);
    SDL_UnlockMutex(m_mutex);
}

PacketQueue::Result PacketQueue::getPacket(Packet &packet, const bool blocking)
{
    Result result = Result::PROGRAM_IS_DONE;
    SDL_LockMutex(m_mutex); 
    for (;;) {
        if (VP::isRunning() == false)
            break;
        if (m_queue.empty() == false) {
            packet = m_queue.front();
            result = Result::EXTRACTED;
            m_queue.pop();
            break;
        } else if (blocking == false) {
            result = Result::EMPTY;
            break; 
        } else {
            SDL_CondWait(m_cond, m_mutex);
        }
    }
    SDL_UnlockMutex(m_mutex);
    return result;
}
