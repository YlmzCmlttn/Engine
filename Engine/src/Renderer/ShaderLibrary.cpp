#include "Renderer/ShaderLibrary.h"
#include "Core/Assert.h"
namespace Engine {

    std::unordered_map<std::string, Ref<Shader>> ShaderLibrary::m_Shaders;

    void ShaderLibrary::add(const Ref<Shader>& shader) {
        ENGINE_CORE_ASSERT(!isExist(shader->getName()), "Shader name already exists");
        m_Shaders[shader->getName()] = shader;
    }

    bool ShaderLibrary::isExist(const std::string& name) {
        return m_Shaders.find(name) != m_Shaders.end();
    }

    Ref<Shader> ShaderLibrary::loadFromFile(const std::string& name, const std::string& filepath){
        ENGINE_CORE_ASSERT(!isExist(name), "Shader name already exists");
        auto shader = Shader::CreateFromFile(name, filepath);
        add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::load(const std::string& name, const std::string& shaderSource){
        ENGINE_CORE_ASSERT(!isExist(name), "Shader name already exists");
        auto shader = Shader::Create(name, shaderSource);
        add(shader);
        return shader;
    }

    Ref<Shader>& ShaderLibrary::get(const std::string& name) {
        ENGINE_CORE_ASSERT(isExist(name), "Shader does not exist");
        return m_Shaders[name];
    }
    
} // namespace Engine