#pragma once

#include "Renderer/RendererAPI.h"

namespace Engine
{    
	enum class TextureFormat
	{
		None = 0,
		RED = 1,
		RGB = 2,
		RGBA = 3,
	};
	class Texture
	{
	public:
		virtual ~Texture() {}
		virtual RendererID getRendererID() const = 0;
        virtual void bind(unsigned int slot = 0) const = 0;

        virtual TextureFormat getFormat() const = 0;
        virtual unsigned int getWidth() const = 0;
        virtual unsigned int getHeight() const = 0;
        virtual const std::string& getPath() const = 0;
	};
} // namespace name
