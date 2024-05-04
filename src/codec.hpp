#pragma once
#include "def.hpp"

extern "C" {
    #include <libavcodec/avcodec.h>
}

namespace VP {
    class Codec final {
    public:
        explicit Codec(const AVCodecID id); 
        Codec(const Codec &other);
        Codec(Codec &&other) noexcept
        {
            m_codec = other.m_codec;
            other.m_codec = nullptr;
        }
        ~Codec() = default;
            
        Codec &operator=(const Codec &rhs)
        { return *this = Codec(rhs); }
        Codec &operator=(Codec &&rhs) noexcept
        { return *this = Codec(std::move(rhs)); }

        const AVCodec *getAVCodec() const noexcept
        { return m_codec; }
    private:
        const AVCodec *m_codec {};
    };
}
