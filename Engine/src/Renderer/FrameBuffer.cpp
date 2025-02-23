#include "Renderer/FrameBuffer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"
#include "Core/Assert.h"
namespace Engine {
    Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec) {        
        switch (RendererAPI::Current()) {
            case RendererAPIType::OpenGL: return CreateRef<OpenGLFrameBuffer>(spec);
        }        
        ENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}