#include "Renderer/Material.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

namespace Engine
{
    Ref<Material> Material::Create(const Ref<Shader>& shader)
	{
		return CreateRef<Material>(shader);
	}

	Material::Material(const Ref<Shader>& shader)
		: m_Shader(shader)
	{
		m_Shader->addShaderReloadedCallback(std::bind(&Material::onShaderReloaded, this));
		allocateStorage();

		m_MaterialFlags |= (uint32_t)MaterialFlag::DepthTest;
		m_MaterialFlags |= (uint32_t)MaterialFlag::Blend;
	}

	Material::~Material()
	{
	}

	void Material::allocateStorage()
	{
		std::cout<<"Allocating storage for material: "<<m_Shader->getName()<<std::endl;
		const auto& shaderBuffers = m_Shader->getShaderUniformBuffers();
        

		for (const auto& [bindingPoint, buffer] : shaderBuffers)
		{
			std::cout<<"Allocating storage for buffer: "<<buffer.name<<std::endl;
			std::cout<<"Buffer size: "<<buffer.size<<std::endl;
			m_UniformStorageBuffers[buffer.name].allocate(buffer.size);
			m_UniformStorageBuffers[buffer.name].setZero();
		}
	}

	void Material::onShaderReloaded()
	{   
		allocateStorage();
		
		for (auto mi : m_MaterialInstances)
			mi->onShaderReloaded();
	}

	bool Material::isUniformBuffer(const std::string& name){
		auto pos = name.find(".");
		if(pos != std::string::npos) return false;
		std::string bufferName = name.substr(0, pos);
		const auto& shaderBuffers = m_Shader->getShaderUniformBuffers();
		for (const auto& [bindingPoint, buffer] : shaderBuffers)
		{
			if (buffer.name == bufferName) return true;
		}
		return false;
	}

	const ShaderUniformBuffer* Material::findUniformBufferDeclaration(const std::string& name)
	{
		const auto& shaderBuffers = m_Shader->getShaderUniformBuffers();
		for (const auto& [bindingPoint, buffer] : shaderBuffers)
		{
			if (buffer.name == name) return &buffer;
		}
		return nullptr;
	}
	const ShaderUniform* Material::findUniformDeclaration(const std::string& name)
	{
		return m_Shader->getUniform(name);
	}

	const ShaderResourceDeclaration* Material::findResourceDeclaration(const std::string& name)
	{
		auto& resources = m_Shader->getResources();
		for (const auto& [n, resource] : resources)
		{
			if (resource.getName() == name)
				return &resource;
		}
		return nullptr;
	}

	void Material::bind()
	{
		auto& shader = m_Shader;
		shader->bind();
		const auto& shaderBuffers = m_Shader->getShaderUniformBuffers();

		for (const auto& [bindingPoint, buffer] : shaderBuffers)
		{
			if (m_UpdatedUniformBuffers.find(buffer.name) != m_UpdatedUniformBuffers.end())
			{
				shader->setUniformBuffer(buffer.name, m_UniformStorageBuffers[buffer.name].data, buffer.size);
			}else{
				for (auto& [name, uniform] : buffer.uniforms)
				{
					const auto& fullname = buffer.name + "." + name;
					if (m_UpdatedUniforms.find(fullname) != m_UpdatedUniforms.end())
					{
						shader->setUniform(fullname, m_UniformStorageBuffers[buffer.name].read(uniform.getOffset()));
					}
				}
			}			
		}
		m_UpdatedUniforms.clear();
		m_UpdatedUniformBuffers.clear();
		bindTextures();
	}

	void Material::bindTextures()
	{
		for (size_t i = 0; i < m_Textures.size(); i++)
		{
			auto& texture = m_Textures[i];
			if (texture)
				texture->bind(i);
		}
	}

	void Material::materialInstancesValueUpdated(const std::string& bufferName, const ShaderUniform& decl)
	{
		for (auto mi : m_MaterialInstances)
			mi->onMaterialValueUpdated(bufferName, decl);
	}
	void Material::materialInstancesValueUpdated(const ShaderUniformBuffer& decl)
	{
		for (auto mi : m_MaterialInstances)
			mi->onMaterialValueUpdated(decl);
	}
} // namespace Engine
