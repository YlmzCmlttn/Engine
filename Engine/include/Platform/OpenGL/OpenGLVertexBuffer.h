#pragma once

#include "Renderer/VertexBuffer.h"
#include "Core/Buffer.h"
namespace Engine {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(void* data, uint32_t size, VertexBufferUsage usage = VertexBufferUsage::Static);
		OpenGLVertexBuffer(uint32_t size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);
		virtual ~OpenGLVertexBuffer();

		virtual void setData(void* buffer, unsigned int size, unsigned int offset = 0);
		virtual void bind() const;

		virtual const BufferLayout& getLayout() const override { return m_Layout; }
		virtual void setLayout(const BufferLayout& layout) override { m_Layout = layout; }

		virtual unsigned int getSize() const { return m_Size; }
		virtual RendererID getRendererID() const { return m_RendererID; }
	private:
		RendererID m_RendererID=0;
		uint32_t m_Size;
		VertexBufferUsage m_Usage;
		BufferLayout m_Layout;

		Buffer m_DataStorage;
	};

}