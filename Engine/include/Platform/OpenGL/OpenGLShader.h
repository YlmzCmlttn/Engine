#pragma once

#include "Renderer/Shader.h"
#include <glad/gl.h>

namespace Engine {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);

		virtual void bind() override;
	private:
		void readShaderFromFile(const std::string& filepath);
		void compileAndUploadShader();
		static GLenum shaderTypeFromString(const std::string& type);
	private:
		RendererID m_RendererID;

		std::string m_ShaderSource;
	};

}