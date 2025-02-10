#pragma once

#include "Core/Core.h"
#include "Core/Assert.h"

namespace Engine {

	struct Buffer
	{
		byte* data;
		uint32_t size;

		Buffer()
			: data(nullptr), size(0)
		{
		}

		Buffer(byte* newData, uint32_t newSize)
			: data(newData), size(newSize)
		{
		}

		static Buffer copy(void* copyData, uint32_t copySize)
		{
			Buffer buffer;
			buffer.allocate(copySize);
			memcpy(buffer.data, copyData, copySize);
			return buffer;
		}

		void allocate(uint32_t newSize)
		{
			delete[] data;
			data = nullptr;

			if (newSize == 0)
				return;

			data = new byte[newSize];
			size = newSize;
		}

		void setZero()
		{
			if (data)
				memset(data, 0, size);
		}

		void write(void* writeData, uint32_t writeSize, uint32_t offset = 0)
		{
			ENGINE_CORE_ASSERT(offset + writeSize <= size, "Buffer overflow!");
			memcpy(data + offset, writeData, writeSize);
		}

		operator bool() const
		{
			return data;
		}

		byte& operator[](int index)
		{
			return data[index];
		}

		byte operator[](int index) const
		{
			return data[index];
		}

		template<typename T>
		T* As()
		{
			return (T*)data;
		}

		inline uint32_t getSize() const { return size; }
	};

}