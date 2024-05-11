#include "packet.hpp"
#include <stdexcept>

extern "C" {
    #include <libavcodec/packet.h>
}
using namespace VP;

Packet::Packet()
{
    const char *err = "Can't allocate packet!\n";
    m_packet = av_packet_alloc(); 
    if (m_packet == nullptr)
        throw std::runtime_error(err);
}

bool Packet::isEmpty() const noexcept
{
    if (m_packet->data == nullptr)
        return true;
    else 
        return false;
}

int Packet::getStreamIndex() const noexcept 
{
    return m_packet->stream_index;
}

void Packet::unref() noexcept 
{
    av_packet_unref(m_packet);
}

Packet::~Packet()
{
    av_packet_free(&m_packet);
}

Packet &Packet::operator=(Packet &&rhs) noexcept
{ 
    if (this != &rhs) {
        if (m_packet != nullptr)
            av_packet_free(&m_packet);
        m_packet       = rhs.m_packet;
        rhs.m_packet   = nullptr;
    }
    return *this;
}

Packet &Packet::operator=(const Packet &rhs)
{ 
    if (this != &rhs) {
        const char *err = "Can't clone packet!\n";
        if (m_packet != nullptr)
            av_packet_free(&m_packet);
        m_packet = av_packet_clone(rhs.m_packet);
        if (m_packet == nullptr)
            throw std::runtime_error(err);
    }
    return *this;
}
