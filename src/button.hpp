#pragma once
#include "label.hpp"
#include "widget.hpp"
#include <memory>

namespace ShGUI {
    class Button final : public Widget {
    public:
        explicit Button(const Rectangle &rect, const Color color = WHITE, const char *text = nullptr);
        ~Button() = default;

        virtual void proccessEvents() override;
        virtual void update() override;
        virtual void draw() const override;
        
        void setLabelColor(const Color color);
    private:
        Color m_saved_button_color;
        Color m_saved_label_color;
        Color m_button_hover_color;
        Color m_label_hover_color;
        Color m_pressed_button_color;
        Color m_pressed_label_color; 
        bool m_hover;
        bool m_pressed;

        std::unique_ptr<Label> m_label;
    };
}
