cmake_minimum_required(VERSION 3.14)
project(vp_test)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    vp_test 
    main.cpp
    ../src/vp.cpp
    ../src/window.cpp
    ../src/renderer.cpp
    ../src/video.cpp
    ../src/codec.cpp 
    ../src/frame.cpp 
    ../src/packet.cpp 
    ../src/texture.cpp 
    ../src/sws_context.cpp 
    ../src/codec_context.cpp 
    ../src/format_context.cpp 
    ../src/swr_context.cpp
    ../src/audio_device.cpp
)

target_link_libraries(
    vp_test 
    GTest::gtest_main
)

target_link_libraries( vp_test SDL2::SDL2-static )
target_link_libraries( vp_test avdevice)
target_link_libraries( vp_test avfilter)
target_link_libraries( vp_test avformat)
target_link_libraries( vp_test avcodec)
target_link_libraries( vp_test swscale)
target_link_libraries( vp_test swresample)
target_link_libraries( vp_test avutil)

include(GoogleTest)
gtest_discover_tests(vp_test)
add_test(vp_test vp_test)
