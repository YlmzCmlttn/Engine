#include "Platform/OpenGL/OpenGLVertexBuffer.h"
#include "Core/Assert.h"
namespace Engine {

	Ref<VertexBuffer> VertexBuffer::Create(void* data, uint32_t size, VertexBufferUsage usage)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    return nullptr;
			case RendererAPIType::OpenGL:  return CreateRef<OpenGLVertexBuffer>(data, size, usage);
		}
		ENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;

	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, VertexBufferUsage usage)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    return nullptr;
			case RendererAPIType::OpenGL:  return CreateRef<OpenGLVertexBuffer>(size, usage);
		}
		ENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}