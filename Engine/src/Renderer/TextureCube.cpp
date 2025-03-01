#include "Renderer/TextureCube.h"
#include "Platform/OpenGL/OpenGLTextureCube.h"
#include "Core/Assert.h"
namespace Engine
{
    Ref<TextureCube> TextureCube::Create(const std::string& path)
    {
        switch (RendererAPI::Current())
        {
            case RendererAPIType::OpenGL: return CreateRef<OpenGLTextureCube>(path);
        }
        ENGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}