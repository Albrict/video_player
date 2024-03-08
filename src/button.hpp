#pragma once
#include "widget.hpp"

namespace ShGUI {
    class Button : public Widget {
    public:
        Button(const Rectangle &rect, const Color color = WHITE, const char *text = nullptr);
        
        virtual void proccessEvents() override;
        virtual void update() override;
        virtual void draw() const override;

    private:
        const char *m_text;
        Color m_saved_color;
        Color m_pressed_color;
        bool m_change_color;
    };
}
