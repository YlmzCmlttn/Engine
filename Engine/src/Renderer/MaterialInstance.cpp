
#include "Renderer/MaterialInstance.h"

namespace Engine
{
    Ref<MaterialInstance> MaterialInstance::Create(const Ref<Material>& material)
	{
		return CreateRef<MaterialInstance>(material);
	}

	MaterialInstance::MaterialInstance(const Ref<Material>& material, const std::string& name)
		: m_Material(material), m_Name(name)
	{
		m_Material->m_MaterialInstances.insert(this);
		allocateStorage();
	}

	MaterialInstance::~MaterialInstance()
	{
		m_Material->m_MaterialInstances.erase(this);
	}

	void MaterialInstance::onShaderReloaded()
	{
		allocateStorage();
		m_OverriddenUniforms.clear();
		m_OverriddenUniformBuffers.clear();
	}

	void MaterialInstance::allocateStorage()
	{
		const auto& shaderBuffers = getShader()->getShaderUniformBuffers();
        

		for (const auto& [bindingPoint, buffer] : shaderBuffers)
		{
			m_UniformStorageBuffers[buffer.name].allocate(buffer.size);
			m_UniformStorageBuffers[buffer.name].setZero();
		}
	}

	void MaterialInstance::setFlag(MaterialFlag flag, bool value)
	{
		if (value)
		{
			m_Material->m_MaterialFlags |= (uint32_t)flag;
		}
		else
		{
			m_Material->m_MaterialFlags &= ~(uint32_t)flag;
		}
	}

	void MaterialInstance::onMaterialValueUpdated(const std::string& bufferName, const ShaderUniform& decl)
	{
		if(m_OverriddenUniforms.find(bufferName+"."+decl.getName()) != m_OverriddenUniforms.end()) return;
		
		m_UpdatedUniforms.insert(bufferName+"."+decl.getName());
		auto& buffer = m_UniformStorageBuffers[bufferName];
		auto& materialBuffer = m_Material->m_UniformStorageBuffers[bufferName];
		buffer.write(materialBuffer.data+decl.getOffset(), decl.getSize(), decl.getOffset());
	}

	void MaterialInstance::onMaterialValueUpdated(const ShaderUniformBuffer& decl)
	{
		if(m_OverriddenUniformBuffers.find(decl.name) != m_OverriddenUniformBuffers.end()) return;
		m_UpdatedUniformBuffers.insert(decl.name);
		auto& buffer = m_UniformStorageBuffers[decl.name];
		auto& materialBuffer = m_Material->m_UniformStorageBuffers[decl.name];
		buffer.write(materialBuffer.data, materialBuffer.size, 0);
	}

	void MaterialInstance::bind()
	{
		auto shader = getShader();
		shader->bind();
		const auto& shaderBuffers = shader->getShaderUniformBuffers();
		for (const auto& [bindingPoint, buffer] : shaderBuffers)
		{
			if (m_UpdatedUniformBuffers.find(buffer.name) != m_UpdatedUniformBuffers.end())
			{
				shader->setUniformBuffer(buffer.name, m_UniformStorageBuffers[buffer.name].data, buffer.size);
			}else{
				for (auto& [name, uniform] : buffer.uniforms)
				{
					if (m_UpdatedUniforms.find(name) != m_UpdatedUniforms.end())
					{
						shader->setUniform(name, m_UniformStorageBuffers[buffer.name].read(uniform.getOffset()));
					}
				}
			}
			
		}
		
		m_UpdatedUniforms.clear();
		m_UpdatedUniformBuffers.clear();

		m_Material->bindTextures();
		for (size_t i = 0; i < m_Textures.size(); i++)
		{
			auto& texture = m_Textures[i];
			if (texture)
				texture->bind(i);
		}
	}
} // namespace Engine
