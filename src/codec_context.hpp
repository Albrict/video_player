#pragma once
#include "def.hpp"
#include <utility>

struct AVCodecContext;
struct AVCodecParameters;

namespace VP {
    class CodecContext {
    public:
        explicit CodecContext(const Codec &codec, AVCodecParameters *parameters);
        CodecContext(CodecContext &&other) noexcept
        { *this = std::move(other); }
        virtual ~CodecContext();
        
        CodecContext &operator=(CodecContext &&rhs) noexcept;

        virtual void sendPacket(const Packet &packet) final;
        virtual void flushBuffers() final;
        virtual int receiveFrame(Frame &frame) final;

        [[nodiscard]] virtual const AVCodecContext *getAVCodecContext() const noexcept final
        { return m_codec_ctx; }
        [[nodiscard]] virtual AVCodecContext *getAVCodecContext() noexcept final
        { return m_codec_ctx; }

        CodecContext(const CodecContext &other) = delete;
        CodecContext &operator=(const CodecContext &rhs) = delete;
    protected:
        AVCodecContext *m_codec_ctx {};
    };
}
