#include "Renderer/IndexBuffer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"
#include "Core/Core.h"

namespace Engine {

	Ref<IndexBuffer> IndexBuffer::Create(void* data,unsigned int size)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    return nullptr;
			case RendererAPIType::OpenGL:  return CreateRef<OpenGLIndexBuffer>(data,size);
		}
		return nullptr;

	}

}