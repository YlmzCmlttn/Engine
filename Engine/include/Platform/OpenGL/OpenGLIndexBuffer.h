#pragma once

#include "Renderer/IndexBuffer.h"
#include "Core/Buffer.h"
namespace Engine {

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(void* data,unsigned int size);
		virtual ~OpenGLIndexBuffer();

		virtual void setData(void* buffer, unsigned int size, unsigned int offset = 0);
		virtual void bind() const;

		virtual unsigned int getCount() const { return m_Size / sizeof(uint32_t); }
		virtual unsigned int getSize() const { return m_Size; }
		virtual RendererID getRendererID() const { return m_RendererID; }
	private:
		RendererID m_RendererID = 0;
		unsigned int m_Size;

		Buffer m_DataStorage;
	};

}