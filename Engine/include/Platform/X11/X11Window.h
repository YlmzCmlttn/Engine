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
        inline void makeContextCurrent() override { m_Context->makeCurrent(); }

        inline void* getNativeWindow() const override { return (void*)m_Window; }
        inline Display* getDisplay() const { return m_Display; }
        inline XVisualInfo* getVisualInfo() const { return m_VisualInfo; }
        void init(const WindowProps& props);

    private:
        void shutdown();
        
        Display* m_Display = nullptr;
        ::Window m_Window = 0;
        XVisualInfo* m_VisualInfo = nullptr;

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
