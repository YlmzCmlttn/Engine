#pragma once

#include "Core/Core.h"
#include "Renderer/VertexArray.h"
#include <glm/glm.hpp>

namespace Engine
{
    class Mesh  
    {
    public:
        struct Vertex {
            glm::vec3 position;
            //glm::vec3 normal;     // Optional: default to glm::vec3(0.0f)
            //glm::vec2 uv;         // Optional: default to glm::vec2(0.0f)
            //glm::vec3 tangent;    // Optional: default to glm::vec3(0.0f)
            //glm::vec3 bitangent;  // Optional: default to glm::vec3(0.0f)

            Vertex(const glm::vec3& pos
                //const glm::vec3& norm = glm::vec3(0.0f),
                //const glm::vec2& tex = glm::vec2(0.0f),
                //const glm::vec3& tang = glm::vec3(0.0f),
                //const glm::vec3& bitang = glm::vec3(0.0f))
                //: position(pos), normal(norm), uv(tex), tangent(tang), bitangent(bitang)
                ): position(pos)
            {}
        };

        Mesh();
        ~Mesh();

        void render();
        void uploadToGPU();

        void setVertices(const std::vector<Vertex>& vertices);
        void setIndices(const std::vector<uint32_t>& indices);

    private:    
        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;
        
        Ref<VertexArray> m_VertexArray;

        BufferLayout m_BufferLayout;

    };
} // namespace Engine