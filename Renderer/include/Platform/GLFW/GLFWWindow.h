#pragma once

#include "Core/Window.h"
#include "Events/Event.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
namespace Engine {
    class GLFWWindow : public Window {
        public:
            GLFWWindow(const WindowProps& props);
            virtual ~GLFWWindow();

            void onUpdate() override;

            inline unsigned int getWidth() const override{return m_Data.width;}
            inline unsigned int getHeight() const override{return m_Data.height;}
            inline float getTime() const override{return (float)glfwGetTime();}

            inline void setEventCallback(const EventCallbackFn& callback) override{m_Data.EventCallback = callback;}
            inline void setVSync(bool enabled) override{m_Data.VSync = enabled;}
            inline bool isVSync() const override{return m_Data.VSync;}
            inline void makeContextCurrent() override{glfwMakeContextCurrent(m_Window);}

            inline void* getNativeWindow() const override{return m_Window;}
        private:
            GLFWwindow* m_Window;
            struct WindowData
            {
                std::string title;
                unsigned int width, height;
                bool VSync;

                EventCallbackFn EventCallback;
            };
            WindowData m_Data;
            void Init(const WindowProps& props);
            void Shutdown();
        
    };
}