#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> 

namespace Engine
{
	struct ShaderUniform
	{
		
	};

	struct ShaderUniformCollection
	{

	};

	enum class UniformType
	{
		None,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int32, UInt32,
		Bool
	};

	struct UniformDecl
	{
		std::string name;
		UniformType type;
		std::ptrdiff_t offset;
	};

	struct UniformBuffer
	{
		unsigned char* data;
		std::vector<UniformDecl> uniforms;
	};

	struct UniformBufferBase
	{
		virtual const unsigned char* getBuffer() const = 0;
		virtual const UniformDecl* getUniforms() const = 0;
		virtual unsigned int getUniformCount() const = 0;
	};

	template<unsigned int Size, unsigned int UniformCount>
	struct UniformBufferDeclaration : UniformBufferBase
	{
		unsigned char buffer[Size];
		UniformDecl uniforms[UniformCount];
		std::ptrdiff_t uniformBufferOffset = 0;
        int uniformIndex = 0;

		virtual const unsigned char* getBuffer() const override { return buffer; }
		virtual const UniformDecl* getUniforms() const override { return uniforms; }
		virtual unsigned int getUniformCount() const override { return UniformCount; }

        void push(const std::string& name, const float& value){
            uniforms[uniformIndex++] = {name, UniformType::Float, uniformBufferOffset};
            std::memcpy(buffer + uniformBufferOffset, &value, sizeof(float));
            uniformBufferOffset += sizeof(float);
        }
		void push(const std::string& name, const glm::vec2& value){
            uniforms[uniformIndex++] = {name, UniformType::Float2, uniformBufferOffset};
            std::memcpy(buffer + uniformBufferOffset, glm::value_ptr(value), sizeof(glm::vec2));
            uniformBufferOffset += sizeof(glm::vec2);
        }
		void push(const std::string& name, const glm::vec3& value){
            uniforms[uniformIndex++] = {name, UniformType::Float3, uniformBufferOffset};
            std::memcpy(buffer + uniformBufferOffset, glm::value_ptr(value), sizeof(glm::vec3));
            uniformBufferOffset += sizeof(glm::vec3);
        }
        void push(const std::string& name, const glm::vec4& value){
            uniforms[uniformIndex++] = {name, UniformType::Float4, uniformBufferOffset};
            std::memcpy(buffer + uniformBufferOffset, glm::value_ptr(value), sizeof(glm::vec4));
            uniformBufferOffset += sizeof(glm::vec4);
        }        
		void push(const std::string& name, const glm::mat4& value){
            uniforms[uniformIndex++] = {name, UniformType::Mat4, uniformBufferOffset};
            std::memcpy(buffer + uniformBufferOffset, glm::value_ptr(value), sizeof(glm::mat4));
            uniformBufferOffset += sizeof(glm::mat4);
        }

        template<typename T>
        void push(const std::string& name, const T& value)
        {         

        }
	};
}

