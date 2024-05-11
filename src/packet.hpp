#pragma once

#include <utility>
struct AVPacket;

namespace VP {
    class Packet final {
    public:
        Packet();
        ~Packet();
        
        Packet(const Packet &other) 
        { *this = other; }
        Packet(Packet &&other) noexcept 
        { *this = std::move(other); }

        Packet &operator=(const Packet &rhs);
        Packet &operator=(Packet &&rhs) noexcept;
        
        void unref() noexcept;
         
        [[nodiscard]] bool isEmpty() const noexcept;
        [[nodiscard]] int getStreamIndex() const noexcept;
        [[nodiscard]] const AVPacket *getAVPacket() const noexcept 
        { return m_packet; }

        [[nodiscard]] AVPacket *getAVPacket() noexcept 
        { return m_packet; }
    private:
        AVPacket *m_packet {};
    };
}
