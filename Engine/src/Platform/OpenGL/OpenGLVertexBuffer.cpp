#include "Platform/OpenGL/OpenGLVertexBuffer.h"

#include <glad/gl.h>

namespace Engine {

	OpenGLVertexBuffer::OpenGLVertexBuffer(unsigned int size)
		: m_RendererID(0), m_Size(size)
	{
		Renderer::Submit([this]() {
			glGenBuffers(1, &m_RendererID);
		});
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		Renderer::Submit([this]() {
			glDeleteBuffers(1, &m_RendererID);
		});
	}

	void OpenGLVertexBuffer::setData(void* buffer, unsigned int size, unsigned int offset)
	{
		Renderer::Submit([this, buffer, size, offset]() {
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
			glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
		});

	}

	void OpenGLVertexBuffer::bind() const
	{
		Renderer::Submit([this]() {
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		});
	}

}