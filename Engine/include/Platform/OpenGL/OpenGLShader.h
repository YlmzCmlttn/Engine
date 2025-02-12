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
		OpenGLShader(const std::string& name, const std::string& shaderSource);
		
		virtual void bind() override;
		virtual void reload() override;
		virtual void uploadUniformBuffer(const UniformBufferBase& uniformBuffer) override;

		virtual void setFloat(const std::string& name, const float& value) override;
		virtual void setFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void setFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void setFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void setMat4(const std::string& name, const glm::mat4& value) override;

		inline const std::string& getName() const override { return m_Name; }
	private:
		void compileAndUploadShader();
		static GLenum shaderTypeFromString(const std::string& type);

		void uploadUniformInt(const std::string& name, const int& value);
		void uploadUniformFloat(const std::string& name, const float& value);
		void uploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void uploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void uploadUniformFloat4(const std::string& name, const glm::vec4& value);
		void uploadUniformMat4(const std::string& name, const glm::mat4& value);


	private:
		RendererID m_RendererID;
		std::string m_FilePath, m_Name;
		std::string m_ShaderSource;
	};

}