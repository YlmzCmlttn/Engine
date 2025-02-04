#pragma once

#include "Events/Event.h"

namespace Engine
{
    struct WindowProps
    {
        std::string title;
        uint32_t width;
        uint32_t height;

        WindowProps(const std::string& title = "Engine", uint32_t width = 1280, uint32_t height = 720)
            : title(title), width(width), height(height) {}
    };

    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() = default;

        virtual void onUpdate() = 0;

        virtual unsigned int getWidth() const = 0;
        virtual unsigned int getHeight() const = 0;
        virtual float getTime() const = 0;

        virtual void setEventCallback(const EventCallbackFn& callback) = 0;
        virtual void setVSync(bool enabled) = 0;
        virtual bool isVSync() const = 0;
        virtual void makeContextCurrent() = 0;

        virtual void* getNativeWindow() const = 0;

        static Window* Create(const WindowProps& props = WindowProps());
    };


}