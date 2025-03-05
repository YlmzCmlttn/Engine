#include "Renderer/Context.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Core/Assert.h"

namespace Engine {
    template<WindowType type>
    Ref<Context<type>> Context<type>::Create(Ref<Window> window) {
        switch (RendererAPI::Current()) {
            case RendererAPIType::OpenGL:  return CreateRef<OpenGLContext<type>>(window);
        }
        ENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
    template class Context<WindowType::GLFW>;
    template class Context<WindowType::X11>;
}
