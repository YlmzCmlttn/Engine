#include "Renderer/IndexBuffer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

#include <glad/gl.h>

namespace Engine {

	OpenGLIndexBuffer::OpenGLIndexBuffer(void* data,unsigned int size)
		: m_RendererID(0), m_Size(size)
	{
		m_DataStorage = Buffer::copy(data,size);
		Renderer::Submit([this]() {
			glCreateBuffers(1, &m_RendererID);
			glNamedBufferData(m_RendererID,m_Size,m_DataStorage.data,GL_STATIC_DRAW);
		});
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		Renderer::Submit([this]() {	
			glDeleteBuffers(1, &m_RendererID);
		});
	}

	void OpenGLIndexBuffer::setData(void* buffer, unsigned int size, unsigned int offset)
	{
		m_DataStorage = Buffer::copy(buffer,size);
		m_Size = size;
		Renderer::Submit([this, offset]() {
			glNamedBufferSubData(m_RendererID, offset, m_Size, m_DataStorage.data);
		});
	}

	void OpenGLIndexBuffer::bind() const
	{
		Renderer::Submit([this]() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_RendererID);
		});
	}

}