#pragma once

#include "Renderer/Texture.h"

namespace Engine
{
    class TextureCube : public Texture
    {
    public:
        static TextureCube* Create(const std::string& path);
    };
} // namespace Engine
