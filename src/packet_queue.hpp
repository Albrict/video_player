#pragma once
#include "packet.hpp"
#include <queue>

struct SDL_mutex;
struct SDL_cond;

namespace VP {

    class PacketQueue {
    public:
        enum class Result {
            PROGRAM_IS_DONE,
            EMPTY,
            EXTRACTED
        };

        PacketQueue();
        ~PacketQueue();

        void putPacket(Packet &packet);
        Result getPacket(Packet &packet, const bool blocking);

    private:
        SDL_mutex         *m_mutex {};        
        SDL_cond          *m_cond  {};
        std::queue<Packet> m_queue {};
    };
}
