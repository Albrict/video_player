#pragma once
#include "raylib.h"
#include <cassert>

namespace ShGUI {
    class Widget {
    public:
        typedef void(callback_function(void *data));

        Widget(const Rectangle &rect, const Color color) noexcept
            : m_widget_rect(rect), m_color(color), m_cb(nullptr), m_data(nullptr) {};
        virtual ~Widget() = default;
        
        virtual void proccessEvents() = 0;
        virtual void update() = 0;
        virtual void draw() const = 0;
          
        Rectangle getRect() const
        { return m_widget_rect; }
        
        void changeRectPosition(const Rectangle &new_position)
        { m_widget_rect = new_position; }
        
        void changeRectPosition(const float x, const float y)
        { m_widget_rect.x = x; m_widget_rect.y = y; }

        void changeColor(const Color color)
        { m_color = color; }

        Color getColor() const
        { return m_color; }

        void setCallback(callback_function *callback, void *data)
        { m_cb = callback; m_data = data; }

        void executeCallback()
        { assert(m_cb != nullptr); m_cb(m_data); }

        bool isCallbackSet()
        { return m_cb != nullptr; }

    private:
        Rectangle m_widget_rect;
        Color m_color;
        callback_function *m_cb;
        void *m_data;
    };
}
