// File: Engine/include/Platform/OpenGL/OpenGLContext.h

#pragma once

#include "Renderer/Context.h"
#include "Platform/GLFW/GLFWWindow.h"
#include <glad/gl.h>
#include <glad/glx.h>
#include <GLFW/glfw3.h>
#include "Core/Assert.h"

namespace Engine {

    template<WindowType type>
    class OpenGLContext : public Context<type> {
    public:
        OpenGLContext(Ref<Window> window);
        virtual ~OpenGLContext();

        virtual void init() override;
        virtual void swapBuffers() override;
        virtual void makeCurrent() override;
    private:
        Ref<Window> m_Window;
        std::conditional_t<type == WindowType::X11, GLXContext, std::nullptr_t> m_GLXContext{};
    };
}