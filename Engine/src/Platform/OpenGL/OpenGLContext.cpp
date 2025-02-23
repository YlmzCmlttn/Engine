// File: Engine/src/Platform/OpenGL/OpenGLContext.cpp

#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/GLFW/GLFWWindow.h"
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
        ENGINE_CORE_ASSERT(window, "Window is null!");
    }

    template<WindowType type>
    void OpenGLContext<type>::init() {
        bool status = false;
        if constexpr (type == WindowType::GLFW) {
            auto glfwWindow = std::dynamic_pointer_cast<GLFWWindow>(m_Window);
            ENGINE_CORE_ASSERT(glfwWindow, "Window is not a GLFWWindow!");

            GLFWwindow* windowHandle = static_cast<GLFWwindow*>(glfwWindow->getNativeWindow());
            ENGINE_CORE_ASSERT(windowHandle, "GLFW window handle is null!");

            glfwMakeContextCurrent(windowHandle);

            status =  LoadOpenGLFunctions((GLADloadfunc)glfwGetProcAddress);
        }
        ENGINE_CORE_ASSERT(status, "Failed to initialize Glad!");
        ENGINE_CORE_INFO("OpenGL Info:");
        //ENGINE_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        //ENGINE_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        //ENGINE_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
    }

    template<WindowType type>
    void OpenGLContext<type>::swapBuffers() {
        if constexpr (type == WindowType::GLFW) {
            auto glfwWindow = std::dynamic_pointer_cast<GLFWWindow>(m_Window);
            ENGINE_CORE_ASSERT(glfwWindow, "Window is not a GLFWWindow!");

            GLFWwindow* windowHandle = static_cast<GLFWwindow*>(glfwWindow->getNativeWindow());
            ENGINE_CORE_ASSERT(windowHandle, "GLFW window handle is null!");

            glfwSwapBuffers(windowHandle);
        }
    }


    // Explicit template instantiation for GLFW
    template class OpenGLContext<WindowType::GLFW>;

}