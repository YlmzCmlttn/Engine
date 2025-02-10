#pragma once

#include "Renderer/RendererAPI.h"

namespace Engine
{    
	enum class TextureFormat
	{
		None = 0,
		RGB = 1,
		RGBA = 2,
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
