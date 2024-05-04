#include "codec.hpp"
#include <stdexcept>

using namespace VP;

Codec::Codec(const AVCodecID id)
    : m_codec(avcodec_find_decoder(id))
{
    const char *error = "Can't find decoder!\n";
    if (m_codec == nullptr)
        throw std::runtime_error(error); 
}


Codec::Codec(const Codec &other)
    : m_codec(avcodec_find_decoder(other.m_codec->id))
{}
