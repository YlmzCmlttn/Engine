#include "Renderer/TextureCube.h"
#include "Platform/OpenGL/OpenGLTextureCube.h"
#include "Core/Assert.h"
namespace Engine
{
    TextureCube* TextureCube::Create(const std::string& path)
    {
        switch (RendererAPI::Current())
        {
            case RendererAPIType::OpenGL: return new OpenGLTextureCube(path);
        }
        ENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}