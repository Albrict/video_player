#pragma once
#include "raylib.h"
#include <cassert>

namespace ShGUI {
    class Widget {
    public:
        typedef void(callback_function(void *data));

        explicit Widget(const Rectangle &rect, const Color color) noexcept
            : m_widget_rect(rect), m_color(color), m_cb(nullptr), m_data(nullptr) {};
        virtual ~Widget() = default;
        
        virtual void proccessEvents() = 0;
        virtual void update() = 0;
        virtual void draw() const = 0;
        
        Rectangle getRect() const noexcept
        { return m_widget_rect; }
        
        const Rectangle &getRectRef() const noexcept
        { return m_widget_rect; }

        void changeRect(const Rectangle &new_rect) noexcept
        { m_widget_rect = new_rect; }
        
        void changeRectPosition(const float x, const float y) noexcept
        { m_widget_rect.x = x; m_widget_rect.y = y; }

        void setColor(const Color color) noexcept
        { m_color = color; }

        Color getColor() const noexcept
        { return m_color; }

        void setCallback(callback_function *callback, void *data) noexcept
        { m_cb = callback; m_data = data; }

        void executeCallback() noexcept
        { assert(m_cb != nullptr); m_cb(m_data); }

        bool isCallbackSet() noexcept
        { return m_cb != nullptr; }

    private:
        Rectangle m_widget_rect;
        Color m_color;
        callback_function *m_cb;
        void *m_data;
    };
}
