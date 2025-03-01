#pragma once

#include "Core/Core.h"
#include "Core/Assert.h"
#include "Core/Buffer.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Texture2D.h"
#include "Renderer/TextureCube.h"
namespace Engine
{
    class MaterialInstance;
    enum class MaterialFlag
	{
		None       = BIT(0),
		DepthTest  = BIT(1),
		Blend      = BIT(2)
	};

	class Material
	{
		friend class MaterialInstance;
	public:
		Material(const Ref<Shader>& shader);
		virtual ~Material();

		void bind();

		uint32_t getFlags() const { return m_MaterialFlags; }
		void setFlag(MaterialFlag flag) { m_MaterialFlags |= (uint32_t)flag; }

		template <typename T>
		void set(const std::string& name, const T& value)
		{
			if(isUniformBuffer(name)){
				const ShaderUniformBuffer* uniformBuffer = findUniformBufferDeclaration(name);
				if(uniformBuffer == nullptr){
					std::cout<<"Uniform buffer not found: "<<name<<std::endl;
					return;
				}
				if(m_UniformStorageBuffers.find(name) == m_UniformStorageBuffers.end()){
					std::cout<<"Uniform buffer not found: "<<uniformBuffer->name<<std::endl;
					return;
				}
				auto& buffer = m_UniformStorageBuffers[name];
				buffer.write((byte*)&value, uniformBuffer->size, 0);
				m_UpdatedUniformBuffers.insert(name);
				materialInstancesValueUpdated(*uniformBuffer);
			}else{
				auto pos = name.find(".");
				auto bufferName = name.substr(0, pos);
				const ShaderUniform* uniform = findUniformDeclaration(name);
				if(m_UniformStorageBuffers.find(bufferName) == m_UniformStorageBuffers.end()){
					std::cout<<"Uniform not found: "<<bufferName<<std::endl;
					return;
				}
				auto& buffer = m_UniformStorageBuffers[bufferName];
				buffer.write((byte*)&value, uniform->getSize(), uniform->getOffset());
				m_UpdatedUniforms.insert(name);
				materialInstancesValueUpdated(bufferName, *uniform);
			}
			//materialInstancesValueUpdated(*decl);
		}

		void set(const std::string& name, const Ref<Texture>& texture)
		{
			auto decl = findResourceDeclaration(name);
			uint32_t slot = decl->getRegister();
			if (m_Textures.size() <= slot)
				m_Textures.resize((size_t)slot + 1);
			m_Textures[slot] = texture;
		}

		void set(const std::string& name, const Ref<Texture2D>& texture)
		{
			set(name, (const Ref<Texture>&)texture);
		}

		void set(const std::string& name, const Ref<TextureCube>& texture)
		{
			set(name, (const Ref<Texture>&)texture);
		}

		template<typename T>
		const T& get(const std::string& name)
		{
			if(isUniformBuffer(name)){
				const ShaderUniformBuffer* uniformBuffer = findUniformBufferDeclaration(name);
				auto& buffer = m_UniformStorageBuffers[uniformBuffer->name];
				return buffer.read<T>(0);
			}else{
				auto pos = name.find(".");
				auto bufferName = name.substr(0, pos);
				const ShaderUniform* decl = findUniformDeclaration(name);
				ENGINE_CORE_ASSERT(decl, "Could not find uniform with name "+ name);
				auto& buffer = m_UniformStorageBuffers[bufferName];
				return buffer.read<T>(decl->getOffset());
			}
		}

		// template<typename T>
		// T& get(const std::string& name)
		// {

		// 	if(isUniformBuffer(name)){
		// 		const ShaderUniformBuffer* uniformBuffer = findUniformBufferDeclaration(name);
		// 		auto& buffer = m_UniformStorageBuffers[uniformBuffer->name];
		// 		m_UpdatedUniformBuffers.insert(name);
		// 		return buffer.read<T>(0);
		// 	}else{
		// 		auto pos = name.find(".");
		// 		auto bufferName = name.substr(0, pos);
		// 		const ShaderUniform* decl = findUniformDeclaration(name);
		// 		ENGINE_CORE_ASSERT(decl, "Could not find uniform with name "+ name);
		// 		auto& buffer = m_UniformStorageBuffers[bufferName];
		// 		m_UpdatedUniforms.insert(name);
		// 		return buffer.read<T>(decl->getOffset());
		// 	}
		// }

		template<typename T>
		Ref<T> getResource(const std::string& name)
		{
			auto decl = findResourceDeclaration(name);
			uint32_t slot = decl->getRegister();
			ENGINE_CORE_ASSERT(slot < m_Textures.size(), "Texture slot is invalid! "+ name);
			return m_Textures[slot];
		}

		Ref<Shader> getShader() { return m_Shader; }
	public:
		static Ref<Material> Create(const Ref<Shader>& shader);
	private:
		void allocateStorage();
		void onShaderReloaded();
		void bindTextures();

		bool isUniformBuffer(const std::string& name);
		const ShaderUniformBuffer* findUniformBufferDeclaration(const std::string& name);
		const ShaderUniform* findUniformDeclaration(const std::string& name);
		const ShaderResourceDeclaration* findResourceDeclaration(const std::string& name);

		void materialInstancesValueUpdated(const std::string& bufferName, const ShaderUniform& decl);
		void materialInstancesValueUpdated(const ShaderUniformBuffer& decl);
	private:
		Ref<Shader> m_Shader;
		std::unordered_set<MaterialInstance*> m_MaterialInstances;

		std::unordered_map<std::string, Buffer> m_UniformStorageBuffers;
		std::unordered_set<std::string> m_UpdatedUniforms;
		std::unordered_set<std::string> m_UpdatedUniformBuffers;

		std::vector<Ref<Texture>> m_Textures;

		uint32_t m_MaterialFlags;
	};
} // namespace Engine
#include "Renderer/MaterialInstance.h"