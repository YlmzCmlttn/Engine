#pragma once

#include "Renderer/Shader.h"
#include "Renderer/Uniforms.h"
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Engine {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);

		virtual void bind() override;
		virtual void uploadUniformBuffer(const UniformBufferBase& uniformBuffer) override;
	private:
		void readShaderFromFile(const std::string& filepath);
		void compileAndUploadShader();
		static GLenum shaderTypeFromString(const std::string& type);

		void uploadUniformFloat(const std::string& name, const float& value);
		void uploadUniformFloat4(const std::string& name, const glm::vec4& value);


	private:
		RendererID m_RendererID;
		std::string m_ShaderSource;
	};

}