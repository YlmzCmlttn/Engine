#pragma once

#include "Renderer.h"

namespace Engine {

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void setData(void* buffer, unsigned int size, unsigned int offset = 0) = 0;
		virtual void bind() const = 0;

		virtual unsigned int getSize() const = 0;
		virtual RendererID getRendererID() const = 0;

		static IndexBuffer* Create(unsigned int size = 0);
	};

}