#include "Renderer/Material.h"

namespace Engine
{
    Material::Material(const std::string& name, Ref<Shader> shader)
        : m_Name(name), m_Shader(shader)
    {
    }

    void Material::bind()
    {
        m_Shader->bind();
    }

    void Material::unbind()
    {
        m_Shader->unbind();
    }

    void Material::setShader(Ref<Shader> shader)
    {
        m_Shader = shader;
    }
} // namespace Engine
