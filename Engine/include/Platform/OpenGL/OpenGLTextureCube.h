#pragma once

#include "Renderer/RendererAPI.h"
#include "Renderer/TextureCube.h"

namespace Engine {

	class OpenGLTextureCube : public TextureCube
	{
	public:
		OpenGLTextureCube(const std::string& path);
		~OpenGLTextureCube();

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
		unsigned char* m_ImageData; // TODO: remove this implement image storage.
		std::string m_Path;
	};
}