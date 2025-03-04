// File: Engine/src/Platform/OpenGL/OpenGLContext.cpp

#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/GLFW/GLFWWindow.h"
#include "Platform/Windows/WindowsWindow.h"
#include <glad/gl.h>
#include <glad/wgl.h>
#ifndef WGL_CONTEXT_MAJOR_VERSION_ARB
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB  0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#endif

#include <GLFW/glfw3.h>
#include "Core/Assert.h"
#include <windows.h>
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
    OpenGLContext<type>::~OpenGLContext(){

        if constexpr (type == WindowType::WINDOWS) {
            if (m_HGLRC){
                wglMakeCurrent(NULL, NULL);
                wglDeleteContext(m_HGLRC);
            }
            auto& hdc = std::dynamic_pointer_cast<WindowsWindow>(m_Window)->getHDC();
            if (hdc){
                ReleaseDC(static_cast<HWND>(m_Window->getNativeWindow()), hdc);
            }
        }
    }


    void* GetAnyGLFuncAddress(const char* name)
    {
        void* p = (void*)wglGetProcAddress(name);
        if (p == 0 ||
            (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
            (p == (void*)-1))
        {
            HMODULE module = LoadLibraryA("opengl32.dll");
            p = (void*)GetProcAddress(module, name);
        }

        return p;
    }


    template<WindowType type>
    void OpenGLContext<type>::init() {
        bool status = false;
        if constexpr (type == WindowType::GLFW) {
            makeCurrent();
            status = LoadOpenGLFunctions((GLADloadfunc)glfwGetProcAddress);
        }
#ifdef _WIN32
        else if constexpr (type == WindowType::WINDOWS) {
            auto windowsWindow = std::dynamic_pointer_cast<WindowsWindow>(m_Window);
            HDC& hdc = windowsWindow->getHDC();

            HGLRC tempContext = wglCreateContext(hdc);
            if (!tempContext) {
                ENGINE_CORE_ERROR("Failed to create temporary OpenGL context!");
                return;
            }

            wglMakeCurrent(hdc, tempContext);

            if (!gladLoadWGL(hdc, (GLADloadfunc)GetAnyGLFuncAddress)) {
                ENGINE_CORE_ERROR("Failed to load WGL functions!");
                return;
            }
            PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
                (PFNWGLCREATECONTEXTATTRIBSARBPROC)GetAnyGLFuncAddress("wglCreateContextAttribsARB");


            int attribs[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
                WGL_CONTEXT_MINOR_VERSION_ARB, 5,
                WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                0
            };

            m_HGLRC = wglCreateContextAttribsARB(hdc, 0, attribs);
            if (!m_HGLRC) {
                ENGINE_CORE_ERROR("Failed to create OpenGL 4.6 Core Profile context!");
                return;
            }
            wglMakeCurrent(nullptr, nullptr);
            wglDeleteContext(tempContext);

            wglMakeCurrent(hdc, m_HGLRC);

            status = LoadOpenGLFunctions((GLADloadfunc)GetAnyGLFuncAddress);
            if (!status) {
                ENGINE_CORE_ERROR("Failed to initialize GLAD!");
            }
        }
#endif

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
        m_Window->swapBuffers();
    }

    template<WindowType type>
    void OpenGLContext<type>::makeCurrent() {
        if constexpr (type == WindowType::GLFW) {
            glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_Window->getNativeWindow()));
        }
        else if constexpr (type == WindowType::WINDOWS) {
            auto& hdc = std::dynamic_pointer_cast<WindowsWindow>(m_Window)->getHDC();
            wglMakeCurrent(hdc, m_HGLRC);
        }        
    }

    // Explicit template instantiation for GLFW
    template class OpenGLContext<WindowType::GLFW>;
    template class OpenGLContext<WindowType::WINDOWS>;

}