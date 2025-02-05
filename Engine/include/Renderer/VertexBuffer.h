#pragma once

#include "Renderer.h"

namespace Engine {

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void setData(void* buffer, unsigned int size, unsigned int offset = 0) = 0;
		virtual void bind() const = 0;

		virtual unsigned int getSize() const = 0;
		virtual RendererID getRendererID() const = 0;

		static VertexBuffer* Create(unsigned int size = 0);
	};

}