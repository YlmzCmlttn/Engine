#pragma once

#include "Core/Core.h"
#include "Core/Window.h"

namespace Engine {

	enum class WindowType {
        GLFW,
        X11
        // Add other window types here
    };
    template<WindowType type>
    class Context {
    public:
        virtual ~Context() = default;
        virtual void init() = 0;
        virtual void swapBuffers() = 0;
        virtual void makeCurrent() = 0;
        static Ref<Context<type>> Create(Ref<Window> window);
    };
}
