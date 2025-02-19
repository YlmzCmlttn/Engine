#pragma once

#include "Core/Core.h"
#include "Renderer/Renderer.h"
#include "Renderer/Uniforms.h"

namespace Engine
{
	class Shader
	{
	public:
		virtual void bind() = 0;
		virtual void unbind() = 0;
		virtual void reload() = 0;

		virtual void uploadUniformBuffer(const UniformBufferBase& uniformBuffer) = 0;
		virtual void setFloat(const std::string& name, const float& value) = 0;
		virtual void setFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void setFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void setFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void setMat4(const std::string& name, const glm::mat4& value) = 0;

		virtual const std::string& getName() const = 0;

		static Ref<Shader> CreateFromFile(const std::string& name, const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& shaderSource);
	};
}