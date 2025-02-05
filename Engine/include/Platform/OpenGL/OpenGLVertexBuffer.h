#pragma once

#include "Renderer/VertexBuffer.h"

namespace Engine {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(unsigned int size);
		virtual ~OpenGLVertexBuffer();

		virtual void setData(void* buffer, unsigned int size, unsigned int offset = 0);
		virtual void bind() const;

		virtual unsigned int getSize() const { return m_Size; }
		virtual RendererID getRendererID() const { return m_RendererID; }
	private:
		RendererID m_RendererID;
		unsigned int m_Size;
	};

}