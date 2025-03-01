#pragma once

#include "Renderer/RendererAPI.h"
#include "Renderer/Texture2D.h"
#include "Core/Buffer.h"
namespace Engine {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(TextureFormat format, unsigned int width, unsigned int height);
		OpenGLTexture2D(const std::string& path,bool srgb);
		~OpenGLTexture2D();

		virtual void bind(unsigned int slot = 0) const;

		virtual TextureFormat getFormat() const { return m_Format; }
		virtual unsigned int getWidth() const { return m_Width; }
		virtual unsigned int getHeight() const { return m_Height; }
		virtual const std::string& getPath() const { return m_Path; }

		virtual RendererID getRendererID() const { return m_RendererID; }

	private:
		RendererID m_RendererID;
		TextureFormat m_Format;
		unsigned int m_Width, m_Height;
		Buffer m_ImageData; // TODO: remove this implement image storage.
		std::string m_Path;
	};
}