#pragma once
#include "Renderer/Material.h"
namespace Engine
{
	class MaterialInstance
	{
		friend class Material;
	public:
		MaterialInstance(const Ref<Material>& material, const std::string& name = "");
		virtual ~MaterialInstance();

		template <typename T>
		void set(const std::string& name, const T& value)
		{
			if(m_Material->isUniformBuffer(name)){
				const ShaderUniformBuffer* uniformBuffer = m_Material->findUniformBufferDeclaration(name);
				auto& buffer = m_UniformStorageBuffers[uniformBuffer->name];
				buffer.write((byte*)&value, uniformBuffer->size, 0);
				m_UpdatedUniformBuffers.insert(name);
				m_OverriddenUniformBuffers.insert(name);
			}else{
				auto pos = name.find(".");
				auto bufferName = name.substr(0, pos);
				const ShaderUniform* decl = m_Material->findUniformDeclaration(name);
				ENGINE_CORE_ASSERT(decl, "Could not find uniform with name "+ name);
				auto& buffer = m_UniformStorageBuffers[bufferName];
				buffer.write((byte*)&value, decl->getSize(), decl->getOffset());
				m_UpdatedUniforms.insert(name);
				m_OverriddenUniforms.insert(name);
			}
			//materialInstancesValueUpdated(*decl);
		}

		void set(const std::string& name, const Ref<Texture>& texture)
		{
			auto decl = m_Material->findResourceDeclaration(name);
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
			if(m_Material->isUniformBuffer(name)){
				const ShaderUniformBuffer* uniformBuffer = m_Material->findUniformBufferDeclaration(name);
				auto& buffer = m_UniformStorageBuffers[uniformBuffer->name];
				return buffer.read<T>(0);
			}else{
				auto pos = name.find(".");
				auto bufferName = name.substr(0, pos);
				const ShaderUniform* decl = m_Material->findUniformDeclaration(name);
				ENGINE_CORE_ASSERT(decl, "Could not find uniform with name "+ name);
				auto& buffer = m_UniformStorageBuffers[bufferName];
				return buffer.read<T>(decl->getOffset());
			}
		}

		// template<typename T>
		// T& get(const std::string& name)
		// {

		// 	if(m_Material->isUniformBuffer(name)){
		// 		const ShaderUniformBuffer* uniformBuffer = m_Material->findUniformBufferDeclaration(name);
		// 		auto& buffer = m_UniformStorageBuffers[uniformBuffer->name];
		// 		m_UpdatedUniformBuffers.insert(name);
		// 		m_OverriddenUniformBuffers.insert(name);
		// 		return buffer.read<T>(0);
		// 	}else{
		// 		auto pos = name.find(".");
		// 		auto bufferName = name.substr(0, pos);
		// 		const ShaderUniform* decl = m_Material->findUniformDeclaration(name);
		// 		ENGINE_CORE_ASSERT(decl, "Could not find uniform with name "+ name);
		// 		auto& buffer = m_UniformStorageBuffers[bufferName];
		// 		m_UpdatedUniforms.insert(name);
		// 		m_OverriddenUniforms.insert(name);
		// 		return buffer.read<T>(decl->getOffset());
		// 	}
		// }

		template<typename T>
		Ref<T> getResource(const std::string& name)
		{
			auto decl = m_Material->findResourceDeclaration(name);
			uint32_t slot = decl->getRegister();
			ENGINE_CORE_ASSERT(slot < m_Textures.size(), "Texture slot is invalid! "+ name);
			return m_Textures[slot];
		}


		void bind();

		uint32_t getFlags() const { return m_Material->m_MaterialFlags; }
		bool getFlag(MaterialFlag flag) const { return (uint32_t)flag & m_Material->m_MaterialFlags; }
		void setFlag(MaterialFlag flag, bool value = true);

		Ref<Shader> getShader() { return m_Material->m_Shader; }

		const std::string& getName() const { return m_Name; }
	public:
		static Ref<MaterialInstance> Create(const Ref<Material>& material);
	private:
		void allocateStorage();
		void onShaderReloaded();
		void onMaterialValueUpdated(const std::string& bufferName, const ShaderUniform& decl);
		void onMaterialValueUpdated(const ShaderUniformBuffer& decl);
	private:
		Ref<Material> m_Material;
		std::string m_Name;

		std::unordered_map<std::string, Buffer> m_UniformStorageBuffers;
		std::unordered_set<std::string> m_UpdatedUniforms;
		std::unordered_set<std::string> m_UpdatedUniformBuffers;
		std::unordered_set<std::string> m_OverriddenUniforms;
		std::unordered_set<std::string> m_OverriddenUniformBuffers;
		std::vector<Ref<Texture>> m_Textures;
	};
} // namespace Engine
