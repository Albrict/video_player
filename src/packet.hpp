#pragma once
#include <utility>

extern "C" {
    #include <libavcodec/packet.h>
}

namespace VP {
    class Packet final {
    public:
        Packet() = default;
        ~Packet() = default;
        
        Packet(const Packet &other) 
        { *this = other; }
        Packet(Packet &&other) noexcept 
        { *this = std::move(other); }

        Packet &operator=(const Packet &rhs)
        {
            if (this != &rhs)
                m_packet = rhs.m_packet;
            return *this;
        }

        Packet &operator=(Packet &&rhs) noexcept
        {
            if (this != &rhs)
               av_packet_move_ref(&m_packet, &rhs.m_packet);
            return *this;
        }
        
        void unref() noexcept 
        { av_packet_unref(&m_packet); }
         
        [[nodiscard]] bool isEmpty() const noexcept 
        { return m_packet.data == nullptr; }
        [[nodiscard]] int getStreamIndex() const noexcept 
        { return m_packet.stream_index; }
        [[nodiscard]] int getSize() const noexcept 
        { return m_packet.size; }

        [[nodiscard]] const AVPacket& getAVPacket() const noexcept 
        { return m_packet; }

        [[nodiscard]] AVPacket& getAVPacket() noexcept 
        { return m_packet; }
    private:
        AVPacket m_packet {};
    };
}
