#include "Renderer/Mesh.h"

#include "Core/Log.h"
#include "Core/Assert.h"
#include "Renderer/Renderer.h"

#include <glad/gl.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>


namespace Engine
{

    Mesh::Mesh()
    {
        m_BufferLayout = BufferLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float3, "a_Normal" },
            { ShaderDataType::Float2, "a_TexCoord" },
            //{ ShaderDataType::Float3, "a_Tangent" },
            //{ ShaderDataType::Float3, "a_Bitangent" }
        });
    }

    Mesh::~Mesh(){}

    void Mesh::setVertices(const std::vector<Vertex>& vertices){
        m_Vertices = vertices;
    }

    void Mesh::setIndices(const std::vector<uint32_t>& indices){
        m_Indices = indices;
    }


    void Mesh::uploadToGPU()
    {
        auto vertexBuffer = VertexBuffer::Create(m_Vertices.data(), static_cast<uint32_t>(m_Vertices.size() * sizeof(Vertex)));
        vertexBuffer->setLayout(m_BufferLayout);
        auto indexBuffer = IndexBuffer::Create(m_Indices.data(), static_cast<uint32_t>(m_Indices.size()) * sizeof(uint32_t));
        m_VertexArray = VertexArray::Create();
        m_VertexArray->addVertexBuffer(vertexBuffer);
        m_VertexArray->setIndexBuffer(indexBuffer);
    }

    void Mesh::render()
    {
        m_VertexArray->bind();
        Renderer::DrawIndexed(m_VertexArray->getIndexBuffer()->getCount(),true);
    }
}