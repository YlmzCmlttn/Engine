#include "Renderer/Shader.h"
#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Core/Assert.h"
namespace Engine {

	Ref<Shader> Shader::CreateFromFile(const std::string& name, const std::string& filepath)
	{
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (!in) {
			ENGINE_CORE_WARN("Could not read shader file {0}", filepath);
			return nullptr;
		}
		std::stringstream buffer;
		buffer << in.rdbuf();
		return Create(name,buffer.str());
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