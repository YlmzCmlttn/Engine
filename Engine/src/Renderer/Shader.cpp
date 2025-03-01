#include "Renderer/Shader.h"
#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Core/Assert.h"
namespace Engine {

	Ref<Shader> Shader::CreateFromFile(const std::string& name, const std::string& filepath)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None: return nullptr;
			case RendererAPIType::OpenGL: return CreateRef<OpenGLShader>(filepath);
		}
		ENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& shaderSource)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None: return nullptr;
			case RendererAPIType::OpenGL: return CreateRef<OpenGLShader>(name, shaderSource);
		}
		ENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}