#pragma once

#include "Renderer/Shader.h"
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spirv_glsl.hpp>

namespace Engine {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& shaderSource);
		
		virtual void bind() override;
		virtual void unbind() override;
		virtual void reload() override;

		virtual RendererID getRendererID() const override { return m_RendererID; }

		virtual void setUniformBuffer(const std::string& name, const void* data, uint32_t size) override;
		virtual void setUniform(const std::string& fullname, void* value) override;
		virtual void setUniform(const std::string& fullname, float value) override;
		virtual void setUniform(const std::string& fullname, int value) override;
		virtual void setUniform(const std::string& fullname, const glm::vec2& value) override;
		virtual void setUniform(const std::string& fullname, const glm::vec3& value) override;
		virtual void setUniform(const std::string& fullname, const glm::vec4& value) override;
		virtual void setUniform(const std::string& fullname, const glm::mat3& value) override;
		virtual void setUniform(const std::string& fullname, const glm::mat4& value) override;

		inline virtual const std::string& getName() const override { return m_Name; }
		virtual const std::unordered_map<uint32_t, ShaderUniformBuffer>& getShaderUniformBuffers() const override { return m_UniformBuffers; }
		virtual const std::unordered_map<std::string, ShaderResourceDeclaration>& getResources() const override { return m_Resources; }

		virtual const ShaderUniform* getUniform(const std::string& name)const override;

		virtual void addShaderReloadedCallback(const ShaderReloadedCallback& callback) override;
	private:
		void load(const std::string& shaderSource,bool forceCompile = true);
		void compile(const std::vector<uint32_t>& vertexBinary, const std::vector<uint32_t>& fragmentBinary);
		void reflect(const std::vector<uint32_t>& binary);


		std::string readShaderFromFile(const std::string& filepath) const;
		std::unordered_map<GLenum, std::string> preProcess(const std::string& source);

		
		bool isCacheFolderExists();
		void createCacheFolder();
		void loadShaders(const std::unordered_map<GLenum, std::string>& shaderSources,bool forceCompile);

		std::optional<std::vector<uint32_t>> readVulkanBinary(GLenum type);
		std::optional<std::vector<uint32_t>> readOpenGLBinary(GLenum type);
		void writeVulkanBinaryToFile(const std::vector<uint32_t>& binary, GLenum type);
		void writeOpenGLBinaryToFile(const std::vector<uint32_t>& binary, GLenum type);
		std::vector<uint32_t> compileGLGS2SPIRV(const std::pair<GLenum, std::string>& shaderSource);
		std::string convertSPIRV2OpenGLSource(const std::vector<uint32_t>& vulkanBinaries);
		std::vector<uint32_t> compileOpenGLSource2SPIRV(const std::string& openGLSource,GLenum type);
		void createProgram();
		GLuint createShader(GLenum type, const std::vector<uint32_t>& binary);
		void linkShaders(std::vector<GLuint>& shaderRendererIDs);

		template<typename T>
        void setUniformT(const std::string& fullname, const T& value);
	private:
		RendererID m_RendererID;
		bool m_Loaded = false;
		bool m_IsCompute = false;
		std::string m_FilePath, m_Name;
		
		std::vector<ShaderReloadedCallback> m_ShaderReloadedCallbacks;
		std::unordered_map<std::string, ShaderResourceDeclaration> m_Resources;
		std::unordered_map<uint32_t, ShaderUniformBuffer> m_UniformBuffers;
	};

}