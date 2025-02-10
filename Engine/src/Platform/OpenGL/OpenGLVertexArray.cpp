#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <glad/gl.h>

#include "Renderer/Renderer.h"

namespace Engine {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return GL_FLOAT;
			case ShaderDataType::Float2:   return GL_FLOAT;
			case ShaderDataType::Float3:   return GL_FLOAT;
			case ShaderDataType::Float4:   return GL_FLOAT;
			case ShaderDataType::Mat3:     return GL_FLOAT;
			case ShaderDataType::Mat4:     return GL_FLOAT;
			case ShaderDataType::Int:      return GL_INT;
			case ShaderDataType::Int2:     return GL_INT;
			case ShaderDataType::Int3:     return GL_INT;
			case ShaderDataType::Int4:     return GL_INT;
			case ShaderDataType::Bool:     return GL_BOOL;
		}

		ENGINE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		Renderer::Submit([this]() {
			glCreateVertexArrays(1, &m_RendererID);
		});
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		Renderer::Submit([this]() {
			glDeleteVertexArrays(1, &m_RendererID);
		});
	}

	void OpenGLVertexArray::bind() const
	{
		Renderer::Submit([this]() {
			glBindVertexArray(m_RendererID);
		});
	}

	void OpenGLVertexArray::unbind() const
	{
		Renderer::Submit([this]() {
			glBindVertexArray(0);
		});
	}

	void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		ENGINE_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(), "Vertex Buffer has no layout!");

		bind();
		vertexBuffer->bind();

		Renderer::Submit([this, vertexBuffer]() {
			const auto& layout = vertexBuffer->getLayout();
			for (const auto& element : layout)
			{
				auto glBaseType = ShaderDataTypeToOpenGLBaseType(element.type);
				glEnableVertexAttribArray(m_VertexBufferIndex);
				if (glBaseType == GL_INT)
				{
					glVertexAttribIPointer(m_VertexBufferIndex,
						element.getComponentCount(),
						glBaseType,
						layout.getStride(),
						(const void*)(intptr_t)element.offset);
				}
				else
				{
					glVertexAttribPointer(m_VertexBufferIndex,
						element.getComponentCount(),
						glBaseType,
						element.normalized ? GL_TRUE : GL_FALSE,
						layout.getStride(),
						(const void*)(intptr_t)element.offset);
				}
				m_VertexBufferIndex++;
			}
		});
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		bind();
		indexBuffer->bind();

		m_IndexBuffer = indexBuffer;
	}

}
