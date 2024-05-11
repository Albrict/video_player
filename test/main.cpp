#include <gtest/gtest.h>
#include "../src/vp.hpp"
#include "../src/window.hpp"
#include "../src/renderer.hpp"
#include "../src/texture.hpp" 
#include "../src/codec.hpp"
#include "../src/video.hpp"

#include <SDL.h>
extern "C" {
    #include <libswscale/swscale.h>
}

TEST(InitTest, SDLInit)
{
    const bool result = VP::init(); 
    EXPECT_EQ(result, true) << "Error on SDL initialization!\n";
    SDL_Quit(); 
}

TEST(InitTest, WindowCreation)
{

    const bool result = VP::init(); 
    EXPECT_NO_THROW(VP::Window window(0, 0, 800, 600, "Title"));
    SDL_Quit(); 
}

TEST(InitTest, WindowInvalidArgs)
{

    const bool result = VP::init(); 
    EXPECT_DEBUG_DEATH(VP::Window window(-1, -1, 800, 600, "Title"), "x and y coordinates are invalid!");
    EXPECT_DEBUG_DEATH(VP::Window window(0, 0, -1, -1, "Title"), "Width and height are invalid!");
    EXPECT_DEBUG_DEATH(VP::Window window(0, 0, 10800, 10600, "Title"), "Width and height bigger than screen!");
    SDL_Quit(); 
}

TEST(InitTest, RenderCreation)
{

    const bool result = VP::init(); 
    VP::Window window(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, "Video player");
    EXPECT_NO_THROW(VP::Renderer render(window));
    SDL_Quit(); 
}

TEST(InitTest, RenderInvalidArgs)
{
    const bool result = VP::init(); 
    VP::Window window(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, "Video player");
    EXPECT_DEBUG_DEATH(VP::Renderer render(window, 511231), "Render driver index is invalid");
    SDL_Quit(); 
}

TEST(TextureTest, CreateTexture)
{
    const bool result = VP::init(); 
    VP::Window   window(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, "Test");
    VP::Renderer render(window);
    
    EXPECT_NO_THROW(VP::Texture texture(render, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 2000, 2000));
    EXPECT_NO_THROW(VP::Texture texture(render, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, 2000, 2000));

    EXPECT_NO_THROW(VP::Texture texture(render, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, 2000, 2000));
    EXPECT_NO_THROW(VP::Texture texture(render, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_TARGET, 2000, 2000));

    EXPECT_NO_THROW(VP::Texture texture(render, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STATIC, 2000, 2000));
    EXPECT_NO_THROW(VP::Texture texture(render, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STATIC, 2000, 2000));
    SDL_Quit(); 
}

TEST(TextureTest, CopyTexture)
{
    const bool result = VP::init(); 
    VP::Window   window(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, "Test");
    VP::Renderer render(window);
    VP::Texture  copy_texture(render, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, 2000, 2000);
    EXPECT_NO_THROW(VP::Texture texture(render, copy_texture));
    SDL_Quit(); 
}

TEST(TextureTest, MoveTexture)
{
    const bool result = VP::init(); 
    VP::Window   window(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, "Test");
    VP::Renderer render(window);
    VP::Texture  move_texture(render, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, 2000, 2000);
    VP::Texture  texture(render, std::move(move_texture));

    EXPECT_EQ(move_texture.SDLTexture(), nullptr);
    SDL_Quit(); 
}

TEST(TextureTest, InvalidArgs)
{
    GTEST_FLAG_SET(death_test_style, "threadsafe");
    const bool result = VP::init(); 
    VP::Window   window(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, "Test");
    VP::Renderer render(window);
    EXPECT_DEBUG_DEATH(VP::Texture texture(render, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, -123, -123), "Width or height are invalid!");
    SDL_Quit(); 
}


TEST(FormatContextTest, InvalidArgs)
{
    EXPECT_DEBUG_DEATH(VP::FormatContext format_context(nullptr), "Path is null!");
    EXPECT_THROW(VP::FormatContext("sdfadsfsd"), std::exception);
}

TEST(FormatContextTest, MoveContext)
{
    VP::FormatContext move_format_context("test.mkv");
    VP::FormatContext format_context(std::move(move_format_context)); 
    EXPECT_EQ(move_format_context.ctx(), nullptr);
    VP::FormatContext another_context = std::move(format_context);
    EXPECT_EQ(format_context.ctx(), nullptr);
}

TEST(CodecTest, CreateCodec)
{
    EXPECT_NO_THROW(VP::Codec codec(AV_CODEC_ID_EAC3));
}

TEST(CodecTest, CopyCodec)
{
    VP::Codec copy_codec(AV_CODEC_ID_EAC3);
    VP::Codec another_codec(AV_CODEC_ID_EAC3);
    VP::Codec codec(copy_codec); 
    
    EXPECT_EQ(copy_codec, codec);

    codec = another_codec;
    EXPECT_EQ(codec, another_codec);
}

TEST(CodecTest, MoveCodec)
{
    VP::Codec move_codec(AV_CODEC_ID_EAC3);
    VP::Codec another_codec(AV_CODEC_ID_EAC3);
    VP::Codec codec(std::move(move_codec));

    EXPECT_EQ(move_codec.getAVCodec(), nullptr);
    
    codec = std::move(another_codec);
    EXPECT_EQ(another_codec.getAVCodec(), nullptr);
}

TEST(CodecContextTest, CodecContextCreation)
{
    VP::FormatContext format_context("test.mkv");
    EXPECT_NO_THROW(VP::CodecContext ctx(format_context.getVideoCodec(), format_context.videoStreamCodecParams()));
}

TEST(CodecContextTest, MoveCodecContext)
{
    VP::FormatContext format_ctx("test.mkv");
    VP::CodecContext  move_ctx(format_ctx.getVideoCodec(), format_ctx.videoStreamCodecParams());
    VP::CodecContext  other_ctx(format_ctx.getVideoCodec(), format_ctx.videoStreamCodecParams());
    VP::CodecContext  ctx(std::move(move_ctx));

    EXPECT_EQ(move_ctx.getAVCodecContext(), nullptr);
    ctx = std::move(other_ctx);
    EXPECT_EQ(other_ctx.getAVCodecContext(), nullptr);
}

TEST(SwsContextTest, SwsCreation)
{
    VP::FormatContext format_ctx("test.mkv");
    VP::CodecContext  codec_ctx(format_ctx.getVideoCodec(), format_ctx.videoStreamCodecParams());
    EXPECT_NO_THROW(VP::SwsContext sws_ctx(codec_ctx, codec_ctx.width(), codec_ctx.height(), AV_PIX_FMT_YUV420P, SWS_BILINEAR));
}

TEST(SwsContextTest, SwsInvalidArgs)
{
    VP::FormatContext format_ctx("test.mkv");
    VP::CodecContext  codec_ctx(format_ctx.getVideoCodec(), format_ctx.videoStreamCodecParams());
    EXPECT_DEBUG_DEATH(VP::SwsContext sws_ctx(codec_ctx, -1, -1, AV_PIX_FMT_YUV420P, SWS_BILINEAR), "Width or height are invalid!");
}

TEST(SwsContextTest, MoveSws)
{
    VP::FormatContext format_ctx("test.mkv");
    VP::CodecContext  codec_ctx(format_ctx.getVideoCodec(), format_ctx.videoStreamCodecParams());
    VP::SwsContext    move_ctx(codec_ctx, codec_ctx.width(), codec_ctx.height(), AV_PIX_FMT_YUV420P, SWS_BILINEAR);
    VP::SwsContext    sws_ctx(std::move(move_ctx));

    EXPECT_EQ(move_ctx.getAVSwsContext(), nullptr);
    move_ctx = std::move(sws_ctx);
    EXPECT_EQ(sws_ctx.getAVSwsContext(), nullptr);
}

TEST(FrameTest, FrameCreation)
{
    GTEST_FLAG_SET(death_test_style, "threadsafe");
    const bool result = VP::init(); 
    VP::Window        window(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, "Test");
    VP::Renderer      render(window);
    VP::FormatContext format_ctx("test.mkv");
    VP::CodecContext  codec_ctx(format_ctx.getVideoCodec(), format_ctx.videoStreamCodecParams());
    EXPECT_NO_THROW(VP::Frame frame(render, codec_ctx.width(), codec_ctx.height()));
    SDL_Quit(); 
}

TEST(FrameTest, FrameInvalidArgs)
{
    GTEST_FLAG_SET(death_test_style, "threadsafe");
    const bool result = VP::init(); 
    VP::Window        window(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, "Test");
    VP::Renderer      render(window);
    VP::FormatContext format_ctx("test.mkv");
    VP::CodecContext  codec_ctx(format_ctx.getVideoCodec(), format_ctx.videoStreamCodecParams());
    EXPECT_DEBUG_DEATH(VP::Frame frame(render, -1, -1), "Width or height are invalid!");
    SDL_Quit(); 
}

TEST(FrameTest, MoveFrame)
{
    GTEST_FLAG_SET(death_test_style, "threadsafe");
    const bool result = VP::init(); 
    VP::Window        window(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, "Test");
    VP::Renderer      render(window);
    VP::FormatContext format_ctx("test.mkv");
    VP::CodecContext  codec_ctx(format_ctx.getVideoCodec(), format_ctx.videoStreamCodecParams());
    VP::Frame         move_frame(render, codec_ctx.width(), codec_ctx.height());
    VP::Frame         frame(std::move(move_frame));
    
    EXPECT_EQ(move_frame.getTexture(), nullptr);
    move_frame = std::move(frame);
    EXPECT_EQ(frame.getTexture(), nullptr);
    SDL_Quit(); 
}

TEST(PacketTest, CreatePacket)
{
    EXPECT_NO_THROW(VP::Packet packet); 
}

TEST(PacketTest, CopyPacket)
{
    VP::Packet copy_packet;
    EXPECT_NO_THROW(VP::Packet packet(copy_packet));
    EXPECT_NO_THROW(VP::Packet packet = copy_packet);
}

TEST(PacketTest, MovePacket)
{
    VP::Packet move_packet;
    VP::Packet packet(std::move(move_packet));
    EXPECT_EQ(move_packet.getAVPacket(), nullptr);
    move_packet = std::move(packet);
    EXPECT_EQ(packet.getAVPacket(), nullptr);
}

TEST(VideoTest, CreateVideo)
{
    GTEST_FLAG_SET(death_test_style, "threadsafe");
    const bool result = VP::init(); 
    VP::Window        window(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, "Test");
    VP::Renderer      render(window);
    EXPECT_NO_THROW(VP::Video video("test.mkv", render));
    SDL_Quit(); 
}

TEST(VideoTest, VideoInvalidArgs)
{
    GTEST_FLAG_SET(death_test_style, "threadsafe");
    const bool result = VP::init(); 
    VP::Window        window(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, "Test");
    VP::Renderer      render(window);
    EXPECT_THROW(VP::Video video("sdffdadf", render), std::exception);
    SDL_Quit(); 
}

TEST(VideoTest, MoveVideo)
{
    GTEST_FLAG_SET(death_test_style, "threadsafe");
    const bool result = VP::init(); 
    VP::Window        window(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, "Test");
    VP::Renderer      render(window);
    VP::Video move_video("test.mkv", render);
    VP::Video video(std::move(move_video));
    
    EXPECT_EQ(move_video.getFormatContext().ctx(), nullptr);
    move_video = std::move(video);
    EXPECT_EQ(video.getFormatContext().ctx(), nullptr);
    SDL_Quit(); 
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
