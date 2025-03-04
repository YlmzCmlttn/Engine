#pragma once
#include "Renderer/Texture2D.h"
namespace Engine{
    class TextureLoader{
    public:
        Ref<Texture2D> LoadTexture(const std::string& filePath);

        static TextureLoader& GetInstance(){
            static TextureLoader instance;
            return instance;
        }

    private:
        TextureLoader() = default;
        TextureLoader(const TextureLoader&) = delete;
        TextureLoader& operator=(const TextureLoader&) = delete;

        std::unordered_map<std::string,Ref<Texture2D>> m_Textures;
    };
}