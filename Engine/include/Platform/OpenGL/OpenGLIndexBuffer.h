#pragma once

#include "Renderer/IndexBuffer.h"

namespace Engine {

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(unsigned int size);
		virtual ~OpenGLIndexBuffer();

		virtual void setData(void* buffer, unsigned int size, unsigned int offset = 0);
		virtual void bind() const;

		virtual unsigned int getSize() const { return m_Size; }
		virtual RendererID getRendererID() const { return m_RendererID; }
	private:
		RendererID m_RendererID;
		unsigned int m_Size;
	};

}
