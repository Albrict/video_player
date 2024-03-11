#pragma once
#include "widget.hpp"

namespace ShGUI {
    class Label final : public Widget {
    public:
        explicit Label(const Rectangle &rect, const Color color, const char *text) noexcept;
        ~Label() = default;
        
        virtual void proccessEvents() override {}
        virtual void update() override {}
        virtual void draw() const override;
    private:
        const char *m_text;
        unsigned int m_font_size;
        unsigned int m_spacing;
    };
}
