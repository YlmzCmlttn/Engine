#pragma once

namespace Engine
{
    class RendererAPI
    {
    public:
        static void clear(float r, float g, float b, float a);
        static void setClearColor(float r, float g, float b, float a);
    };
} // namespace Engine