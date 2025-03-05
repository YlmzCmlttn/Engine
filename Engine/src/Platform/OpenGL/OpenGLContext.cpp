// File: Engine/src/Platform/OpenGL/OpenGLContext.cpp

#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/GLFW/GLFWWindow.h"
#include "Platform/X11/X11Window.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Core/Assert.h"

namespace Engine {

    static bool LoadOpenGLFunctions(GLADloadfunc loadfunc) {
        int status = gladLoadGL(loadfunc);
        return status != 0;
    }

    template<WindowType type>
    OpenGLContext<type>::OpenGLContext(Ref<Window> window)
        : m_Window(window) {
        
    }

    template<WindowType type>
    OpenGLContext<type>::~OpenGLContext(){
        if constexpr (type == WindowType::X11) {
            auto x11Window = std::dynamic_pointer_cast<X11Window>(m_Window);
            ENGINE_CORE_ASSERT(x11Window, "Window is not a X11Window!");

            X11Window* windowHandle = static_cast<X11Window*>(x11Window->getNativeWindow());
            glXMakeCurrent(windowHandle->getDisplay(), None, nullptr);
            glXDestroyContext(windowHandle->getDisplay(), m_GLXContext);
        }
    }

    template<WindowType type>
    void OpenGLContext<type>::init() {
        bool status = false;
        if constexpr (type == WindowType::GLFW) {
            makeCurrent();
            status =  LoadOpenGLFunctions((GLADloadfunc)glfwGetProcAddress);
        }
        if constexpr (type == WindowType::X11) {
            auto x11Window = std::dynamic_pointer_cast<X11Window>(m_Window);
            ENGINE_CORE_ASSERT(x11Window, "Window is not a X11Window!");

            m_GLXContext = glXCreateContext(x11Window->getDisplay(), x11Window->getVisualInfo(), nullptr, GL_TRUE);
            ENGINE_CORE_ASSERT(m_GLXContext, "Failed to create GLX context!");

            makeCurrent();
            status = LoadOpenGLFunctions((GLADloadfunc)glXGetProcAddress);
        }
        if (!status) {
            ENGINE_CORE_ERROR("Failed to initialize GLAD!");
        }
        std::string vendor = std::string((char*)glGetString(GL_VENDOR));
        std::string renderer = std::string((char*)glGetString(GL_RENDERER));
        std::string version = std::string((char*)glGetString(GL_VERSION));
        ENGINE_CORE_ASSERT(status, "Failed to initialize Glad!");
        ENGINE_CORE_INFO("OpenGL Info:");
        ENGINE_CORE_INFO("  Vendor: {0}", vendor);
        ENGINE_CORE_INFO("  Renderer: {0}", renderer);
        ENGINE_CORE_INFO("  Version: {0}", version);
    }

    template<WindowType type>
    void OpenGLContext<type>::swapBuffers() {
        if constexpr (type == WindowType::GLFW) {
            auto glfwWindow = std::dynamic_pointer_cast<GLFWWindow>(m_Window);
            ENGINE_CORE_ASSERT(glfwWindow, "Window is not a GLFWWindow!");

            GLFWwindow* windowHandle = static_cast<GLFWwindow*>(glfwWindow->getNativeWindow());
            ENGINE_CORE_ASSERT(windowHandle, "GLFW window handle is null!");

            glfwSwapBuffers(windowHandle);
            return;
        }
        if constexpr (type == WindowType::X11) {
            auto x11Window = std::dynamic_pointer_cast<X11Window>(m_Window);
            ENGINE_CORE_ASSERT(x11Window, "Window is not a X11Window!");

            ::Window windowHandle = reinterpret_cast<::Window>(x11Window->getNativeWindow());
            ENGINE_CORE_ASSERT(windowHandle != 0, "X11 window handle is null!");

            glXSwapBuffers(x11Window->getDisplay(), windowHandle);
            return;
        }
    }

    template<WindowType type>
    void OpenGLContext<type>::makeCurrent() {
        if constexpr (type == WindowType::GLFW) {
            glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_Window->getNativeWindow()));
            return;
        }
        if constexpr (type == WindowType::X11) {
            auto x11Window = std::dynamic_pointer_cast<X11Window>(m_Window);
            ENGINE_CORE_ASSERT(x11Window, "Window is not a X11Window!");

            glXMakeCurrent(x11Window->getDisplay(), *static_cast<::Window*>(x11Window->getNativeWindow()), m_GLXContext);
            return;
        }        
    }


    // Explicit template instantiation for GLFW
    template class OpenGLContext<WindowType::GLFW>;
    template class OpenGLContext<WindowType::X11>;

}