#pragma once
#include <SDL_error.h>
#include <iostream>
#include <stdexcept>

extern "C" {
    #include <libavutil/error.h>
}

namespace VP {
    inline void check_libav_return_value(const int result) 
    {
        const size_t string_size     = 128;
        const char   *enum_not_found = "Can't find description value!\n";
        std::string string;
        string.resize(128);
        if (result < 0) {
            int err = av_strerror(result, string.data(), 128);
            if (err < 0) 
                std::cerr << enum_not_found << '\n';
            else
                throw std::runtime_error(string);
        }
    }

    inline void check_sdl_return_value(const int result)
    {
        if (result < 0)
            throw std::runtime_error(SDL_GetError());
    }
}
