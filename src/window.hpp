#pragma once
#include <raylib.h>

namespace VideoPlayer {
    class Window {
    public:
        Window(const unsigned int width, const unsigned int height, const char *title);
        ~Window()
        { CloseWindow(); }
    private:
        unsigned int m_width = 0;
        unsigned int m_height = 0;
        const char *m_title;
    };
}
