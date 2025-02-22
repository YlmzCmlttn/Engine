#include "Renderer/VertexArray.h"

#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Engine {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:    ENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPIType::OpenGL:  return std::make_shared<OpenGLVertexArray>();
		}

		ENGINE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}