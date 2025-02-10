#pragma once

#include "Core/Core.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include <glm/glm.hpp>

namespace Engine
{
    class Mesh  
    {
    public:
        struct Vertex
        {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec3 tangent;
            glm::vec3 bitangent;
            glm::vec2 uv;
        };

        struct Index
        {
            uint32_t i1;
            uint32_t i2;
            uint32_t i3;
        };

        Mesh(const std::string& path);
        ~Mesh();

        void render();

        inline const std::string& getFilePath() const { return m_FilePath; }

    private:
        std::vector<Vertex> m_Vertices;
        std::vector<Index> m_Indices;

        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;

        std::string m_FilePath;        
    };
} // namespace Engine