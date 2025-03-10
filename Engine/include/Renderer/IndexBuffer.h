#pragma once

#include "Renderer.h"
#include "Core/Core.h"

namespace Engine {

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void setData(void* buffer, unsigned int size, unsigned int offset = 0) = 0;
		virtual void bind() const = 0;

		virtual uint32_t getCount() const = 0;
		virtual uint32_t getSize() const = 0;
		virtual RendererID getRendererID() const = 0;

		static Ref<IndexBuffer> Create(void* data,unsigned int size = 0);
	};

}