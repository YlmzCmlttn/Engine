#include "Renderer/IndexBuffer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

#include <glad/gl.h>

namespace Engine {

	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int size)
		: m_RendererID(0), m_Size(size)
	{
		Renderer::Submit([this]() {
			glGenBuffers(1, &m_RendererID);
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
		Renderer::Submit([this, buffer, size, offset]() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
		});
	}

	void OpenGLIndexBuffer::bind() const
	{
		Renderer::Submit([this]() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		});
	}

}