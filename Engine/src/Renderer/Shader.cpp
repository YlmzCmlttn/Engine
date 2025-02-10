#include "Renderer/Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Engine {

	std::vector<Shader*> Shader::s_Shaders;

	Shader* Shader::Create(const std::string& filepath)
	{
		Shader* shader = nullptr;
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None: return nullptr;
			case RendererAPIType::OpenGL: shader = new OpenGLShader(filepath); break;
		}
		s_Shaders.push_back(shader);
		return shader;
	}

}