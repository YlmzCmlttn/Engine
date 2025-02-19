#pragma once

#include "Renderer/Shader.h"

namespace Engine
{
    class Material
    {
    public:
        Material(const std::string& name, Ref<Shader> shader);

        void bind();
        void unbind();

        void setShader(Ref<Shader> shader);

        inline const std::string& getName() const { return m_Name; }
        inline Ref<Shader> getShader() const { return m_Shader; }
        
    private:
        std::string m_Name;
        Ref<Shader> m_Shader;
    };
} // namespace name
