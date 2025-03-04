#include "Loader/TextureLoader.h"
namespace Engine{
    Ref<Texture2D> TextureLoader::LoadTexture(const std::string& filePath){
        if(m_Textures.find(filePath) != m_Textures.end()){
            return m_Textures[filePath];
        }
        auto texture = Texture2D::Create(filePath);
        m_Textures[filePath] = texture;
        return texture;
    }
}