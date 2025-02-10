#include "Renderer/FrameBuffer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"
namespace Engine {
    Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec) {        
        switch (RendererAPI::Current()) {
            case RendererAPIType::OpenGL: return CreateRef<OpenGLFrameBuffer>(spec);
        }        
    }
}