#include "Platform/OpenGL/OpenGLVertexBuffer.h"
#include "Renderer/Renderer.h"
#include <glad/gl.h>

namespace Engine {

	static GLenum OpenGLUsage(VertexBufferUsage usage)
	{
		switch (usage)
		{
			case VertexBufferUsage::Static:    return GL_STATIC_DRAW;
			case VertexBufferUsage::Dynamic:   return GL_DYNAMIC_DRAW;
		}
		ENGINE_CORE_ASSERT(false, "Unknown vertex buffer usage");
		return 0;
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(void* data, uint32_t size, VertexBufferUsage usage)
		: m_RendererID(0),m_Size(size), m_Usage(usage)
	{
		m_DataStorage = Buffer::copy(data,size);
		Renderer::Submit([this]() {
			glCreateBuffers(1, &m_RendererID);
			glNamedBufferData(m_RendererID,m_Size,m_DataStorage.data,OpenGLUsage(m_Usage));
		});
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size, VertexBufferUsage usage)
		: m_RendererID(0),m_Size(size), m_Usage(usage)
	{
		Renderer::Submit([this]() {
			glCreateBuffers(1, &m_RendererID);
			glNamedBufferData(m_RendererID, m_Size, nullptr, OpenGLUsage(m_Usage));
		});
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		Renderer::Submit([this]() {
			glDeleteBuffers(1, &m_RendererID);
		});
	}

	void OpenGLVertexBuffer::setData(void* data, uint32_t size, uint32_t offset)
	{
		m_DataStorage = Buffer::copy(data, size);
		m_Size = size;
		Renderer::Submit([this, offset]() {
			glNamedBufferSubData(m_RendererID, offset, m_Size, m_DataStorage.data);
		});

	}

	void OpenGLVertexBuffer::bind() const
	{
		Renderer::Submit([this]() {
			glBindBuffer(GL_ARRAY_BUFFER, this->m_RendererID);

		});
	}

}