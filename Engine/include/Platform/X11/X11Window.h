#pragma once
#include "Core/Window.h"
#include "Events/Event.h"
#include "Renderer/Context.h"
#include <string>
#include <X11/Xlib.h>     // For Display and Window types
#include <GL/glx.h>       // For GLXContext

namespace Engine {

    class X11Window : public Window, public std::enable_shared_from_this<X11Window> {
    public:
        X11Window(const WindowProps& props);
        virtual ~X11Window();

        void onUpdate() override;
        void processEvents() override;
        void swapBuffers() override;

        inline unsigned int getWidth() const override { return m_Data.width; }
        inline unsigned int getHeight() const override { return m_Data.height; }
        inline float getTime() const override;

        inline void setEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        inline void setVSync(bool enabled) override { m_Data.VSync = enabled; }
        inline bool isVSync() const override { return m_Data.VSync; }
        inline void makeContextCurrent() override { glXMakeCurrent(m_Display, m_Window, m_GLXContext); }

        inline void* getNativeWindow() const override { return (void*)m_Window; }
        void init(const WindowProps& props);

    private:
        void shutdown();

        // Native X11 members
        Display* m_Display = nullptr;
        ::Window m_Window = 0;
        GLXContext m_GLXContext = 0;

        // Our engine's rendering context wrapper for X11 (assumed similar to GLFW's)
        Ref<Context<WindowType::X11>> m_Context;

        struct WindowData {
            std::string title;
            unsigned int width, height;
            bool VSync;
            EventCallbackFn EventCallback;
        };
        WindowData m_Data;
    };

} // namespace Engine
