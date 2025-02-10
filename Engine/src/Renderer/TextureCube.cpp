#include "Renderer/TextureCube.h"
#include "Platform/OpenGL/OpenGLTextureCube.h"

namespace Engine
{
    TextureCube* TextureCube::Create(const std::string& path)
    {
        switch (RendererAPI::Current())
        {
            case RendererAPIType::OpenGL: return new OpenGLTextureCube(path);
        }
    }
}