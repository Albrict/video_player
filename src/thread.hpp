#pragma once
#include <SDL_thread.h>
#include <stdexcept>

namespace VP {
    class Thread {
    public:
        Thread(SDL_ThreadFunction function, const char *thread_name, void *data)
        { 
            m_thread = SDL_CreateThread(function, thread_name, data);
            if (m_thread == nullptr)
                throw std::runtime_error(SDL_GetError());
        }
        ~Thread() = default;
        
    private:
        SDL_Thread *m_thread {};
    };
}
