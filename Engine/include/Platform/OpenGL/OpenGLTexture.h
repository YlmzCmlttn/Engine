#pragma once

#include "Renderer/RendererAPI.h"
#include "Renderer/Texture.h"

namespace Engine {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(TextureFormat format, unsigned int width, unsigned int height);
		~OpenGLTexture2D();

		virtual TextureFormat getFormat() const { return m_Format; }
		virtual unsigned int getWidth() const { return m_Width; }
		virtual unsigned int getHeight() const { return m_Height; }
	private:
		RendererID m_RendererID;
		TextureFormat m_Format;
		unsigned int m_Width, m_Height;
	};
}