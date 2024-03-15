#pragma once
#include <SDL_error.h>
#include <stdexcept>

extern "C" {
    #include <libavutil/error.h>
}

namespace VP {
    template<class Callable, class... Args>
    void check_call(Callable t_callable, Args&&... t_args)
    {
        std::string string;
        string.resize(128);
        int ret = t_callable(std::forward<Args>(t_args)...);
        if(ret < 0) {
            int err = av_strerror(ret, string.data(), 128);
            throw std::runtime_error(string);
        }
    }

    template<class Obj, class Callable, class... Args>
    Obj check_callnullptr(const char *err_msg, Callable callable, Args&&... args)
    {
        Obj obj = callable(std::forward<Args>(args)...);
        if(obj == nullptr ) 
            throw std::runtime_error(err_msg);
        else
            return obj;
    }

    template<class Obj, class Callable, class... Args>
    Obj check_SDLnullptr(Callable callable, Args&&... args)
    {
        Obj obj = callable(std::forward<Args>(args)...);
        if(obj == nullptr ) 
            throw std::runtime_error(SDL_GetError());
        else
            return obj;
    }
}
