#pragma once
#include "def.hpp"

extern "C" {
    #include <libavcodec/avcodec.h>
}

namespace VP {
    class Codec final {
    public:
        explicit Codec(const AVCodecID id); 
        Codec(const Codec &other)
        { *this = other; }

        Codec(Codec &&other) noexcept
        { *this = std::move(other); }

        ~Codec() = default;
            
        Codec &operator=(const Codec &rhs)
        { 
            if (this != &rhs)
                this->m_codec = avcodec_find_decoder(rhs.m_codec->id);
            return *this;
        }

        Codec &operator=(Codec &&rhs) noexcept
        { 
            this->m_codec = rhs.m_codec;
            rhs.m_codec   = nullptr;
            return *this;
        }
       
        friend constexpr bool operator==(const Codec &lhs, const Codec &rhs)
        {
            if (lhs.m_codec->id == rhs.m_codec->id)
                return true;
            else
                return false;
        }

        const AVCodec *getAVCodec() const noexcept
        { return m_codec; }
    private:
        const AVCodec *m_codec {};
    };
}
