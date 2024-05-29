#pragma once
#include "def.hpp"
#include <cstdint>
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

        virtual int  sendPacket(const Packet &packet) final;
        virtual void flushBuffers() final;
        virtual int receiveFrame(Frame &frame) final;

        [[nodiscard]] virtual const AVCodecContext *getAVCodecContext() const noexcept final
        { return m_codec_ctx; }
        [[nodiscard]] virtual AVCodecContext *getAVCodecContext() noexcept final
        { return m_codec_ctx; }
        [[nodiscard]] virtual int64_t guess_correct_pts(const int64_t reordered_pts, int64_t dts) const noexcept final;

        CodecContext(const CodecContext &other) = delete;
        CodecContext &operator=(const CodecContext &rhs) = delete;
    protected:
        AVCodecContext *m_codec_ctx {};
    };
}
