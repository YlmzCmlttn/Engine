// File: Engine/include/Platform/OpenGL/OpenGLContext.h

#pragma once

#include "Renderer/Context.h"
#include "Platform/GLFW/GLFWWindow.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "Core/Assert.h"

namespace Engine {

    template<WindowType type>
    class OpenGLContext : public Context<type> {
    public:
        OpenGLContext(Ref<Window> window);
        virtual ~OpenGLContext() = default;

        virtual void init() override;
        virtual void swapBuffers() override;

    private:
        Ref<Window> m_Window;
    };

}