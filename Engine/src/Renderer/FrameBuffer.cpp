#include "Renderer/FrameBuffer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"
namespace Engine {
    Ref<FrameBuffer> FrameBuffer::Create(FrameBufferFormat format, unsigned int width, unsigned int height) {        
        switch (RendererAPI::Current()) {
            case RendererAPIType::OpenGL: return CreateRef<OpenGLFrameBuffer>(format, width, height);
        }        
    }
}