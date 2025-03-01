#pragma once

#include "Core/Core.h"
#include "Renderer/Renderer.h"
#include <glm/glm.hpp>
#include "Renderer/ShaderUniform.h"
namespace Engine
{
	class Shader
	{
	public:
		using ShaderReloadedCallback = std::function<void()>;
		virtual void bind() = 0;
		virtual void unbind() = 0;
		virtual void reload() = 0;

		virtual RendererID getRendererID() const = 0;
		
		virtual void setUniformBuffer(const std::string& name, const void* data, uint32_t size) = 0;
		virtual void setUniform(const std::string& fullname, void* value) = 0;
		virtual void setUniform(const std::string& fullname, float value) = 0;
		virtual void setUniform(const std::string& fullname, int value) = 0;
		virtual void setUniform(const std::string& fullname, const glm::vec2& value) = 0;
		virtual void setUniform(const std::string& fullname, const glm::vec3& value) = 0;
		virtual void setUniform(const std::string& fullname, const glm::vec4& value) = 0;
		virtual void setUniform(const std::string& fullname, const glm::mat3& value) = 0;
		virtual void setUniform(const std::string& fullname, const glm::mat4& value) = 0;

		virtual const std::string& getName() const = 0;

		virtual const std::unordered_map<uint32_t, ShaderUniformBuffer>& getShaderUniformBuffers() const = 0;
		virtual const std::unordered_map<std::string, ShaderResourceDeclaration>& getResources() const = 0;

		virtual void addShaderReloadedCallback(const ShaderReloadedCallback& callback) = 0;
		virtual const ShaderUniform* getUniform(const std::string& name) const = 0;
		static Ref<Shader> CreateFromFile(const std::string& name, const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& shaderSource);
	};
}