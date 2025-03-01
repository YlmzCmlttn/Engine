#pragma once

#include "Renderer/Texture.h"
#include "Core/Core.h"
namespace Engine
{
    class TextureCube : public Texture
    {
    public:
        static Ref<TextureCube> Create(const std::string& path);
    };
} // namespace Engine
