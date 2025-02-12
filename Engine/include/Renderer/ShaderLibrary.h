#pragma once

#include "Renderer/Shader.h"
#include "Core/Core.h"
namespace Engine {

    class ShaderLibrary {
        public:
            static void add(const Ref<Shader>& shader);
            static Ref<Shader> loadFromFile(const std::string& name, const std::string& filepath);
            static Ref<Shader> load(const std::string& name, const std::string& shaderSource);
            static Ref<Shader> get(const std::string& name);
            static bool isExist(const std::string& name);
        private:
            static std::unordered_map<std::string, Ref<Shader>> m_Shaders;
    };    
} // namespace Engine