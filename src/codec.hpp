#pragma once
#include "def.hpp"

extern "C" {
    #include <libavcodec/avcodec.h>
}

namespace VP {
    class Codec {
    public:
        Codec(const AVCodecID id); 
        ~Codec() = default;
        
        const AVCodec *getAVCodec() const noexcept
        { return m_codec; }
    private:
        const AVCodec *m_codec {};
    };
}
