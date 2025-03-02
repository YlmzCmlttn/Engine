#pragma once

#include "Core/Window.h"
#include "Events/Event.h"
#include "Renderer/Context.h"
#include <string>
#include <Windows.h>

namespace Engine {
    class WindowsWindow : public Window, public std::enable_shared_from_this<WindowsWindow>
    {
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        void onUpdate() override;
        void processEvents() override;
        void swapBuffers() override;

        inline unsigned int getWidth() const override { return m_Data.width; }
        inline unsigned int getHeight() const override { return m_Data.height; }
        inline float getTime() const override { return (float)GetTickCount() / 1000.0f; }

        inline void setEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        inline void setVSync(bool enabled) override { m_Data.VSync = enabled; /* Optionally set VSync via the context */ }
        inline bool isVSync() const override { return m_Data.VSync; }
        inline void makeContextCurrent() override { m_Context->makeCurrent();}

        inline void* getNativeWindow() const override { return (void*)m_Window; }
        inline HDC& getHDC() { return m_HDC; }

        inline WindowType getWindowType()const {return WindowType::WINDOWS;}

        void init(const WindowProps& props);
        void shutdown();

    private:
        HWND m_Window;
        HINSTANCE m_HInstance;
        HDC m_HDC;

        
        Ref<Context<WindowType::WINDOWS>> m_Context;

        struct WindowData
        {
            std::string title;
            unsigned int width, height;
            bool VSync;

            EventCallbackFn EventCallback;
        };
        WindowData m_Data;

        // The WindowProc function handles messages for this window.
        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    };
} // namespace Engine
