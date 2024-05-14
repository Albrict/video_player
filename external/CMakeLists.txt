set( BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE )

# SDL
# https://github.com/libsdl-org/SDL
set( SDL_LIBC ON CACHE BOOL "" FORCE )
set( SDL_TEST OFF CACHE BOOL "" FORCE )
add_subdirectory( sdl )
target_link_libraries( main SDL2::SDL2main SDL2::SDL2-static )

# SDL_image
# https://github.com/libsdl-org/SDL_image
set( SDL2IMAGE_SAMPLES OFF CACHE BOOL "" FORCE )
set( SDL2IMAGE_INSTALL OFF CACHE BOOL "" FORCE )
set( SDL2IMAGE_VENDORED ON CACHE BOOL "" FORCE )
set( SDL2IMAGE_DEPS_SHARED OFF CACHE BOOL "" FORCE )
add_subdirectory( sdl-image )
target_link_libraries( main SDL2_image::SDL2_image-static )

# SDL_ttf
# https://github.com/libsdl-org/SDL_ttf
set( SDL2TTF_SAMPLES OFF CACHE BOOL "" FORCE )
set( SDL2TTF_INSTALL OFF CACHE BOOL "" FORCE )
set( SDL2TTF_VENDORED ON CACHE BOOL "" FORCE )
set( SDL2TTF_HARFBUZZ ON CACHE BOOL "" FORCE )
add_subdirectory( sdl-ttf )
target_link_libraries( main SDL2_ttf::SDL2_ttf-static )

target_link_libraries( main avdevice)
target_link_libraries( main avfilter)
target_link_libraries( main avformat)
target_link_libraries( main avcodec)
target_link_libraries( main swscale)
target_link_libraries( main swresample)
target_link_libraries( main avutil)
