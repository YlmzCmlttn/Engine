#include "Platform/OpenGL/OpenGLTexture2D.h"

#include "Platform/OpenGL/OpenGLTexture.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/Renderer.h"

#include <glad/gl.h>
#include <stb_image.h>
namespace Engine {

	OpenGLTexture2D::OpenGLTexture2D(TextureFormat format, unsigned int width, unsigned int height)
		: m_Format(format), m_Width(width), m_Height(height)
	{
		Renderer::Submit([this]() {
			glGenTextures(1, &this->m_RendererID);
			glBindTexture(GL_TEXTURE_2D, this->m_RendererID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTextureParameterf(this->m_RendererID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);



			glTexImage2D(GL_TEXTURE_2D, 0, TextureFormatToOpenGL(this->m_Format), this->m_Width, this->m_Height, 0, TextureFormatToOpenGL(this->m_Format), GL_UNSIGNED_BYTE, nullptr);
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);
		});
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path,bool srgb)
	{
		int width, height, channels;
		m_ImageData = stbi_load(path.c_str(), &width, &height, &channels, 0);
		m_Width = width;
		m_Height = height;
		m_Format = (channels == 4) ? TextureFormat::RGBA : TextureFormat::RGB;

		Renderer::Submit([this,srgb]() {
			if(srgb){
				glCreateTextures(GL_TEXTURE_2D, 1, &this->m_RendererID);
				int levels = CalculateMipMapCount(this->m_Width, this->m_Height);
				glTextureStorage2D(this->m_RendererID, levels, GL_SRGB8, this->m_Width, this->m_Height);
				glTextureParameteri(this->m_RendererID, GL_TEXTURE_MIN_FILTER, levels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
				glTextureParameteri(this->m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glTextureSubImage2D(this->m_RendererID, 0, 0, 0, this->m_Width, this->m_Height, GL_RGB, GL_UNSIGNED_BYTE, this->m_ImageData);
				glGenerateTextureMipmap(this->m_RendererID);
			}else{
				glGenTextures(1, &this->m_RendererID);
				glBindTexture(GL_TEXTURE_2D, this->m_RendererID);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				glTexImage2D(GL_TEXTURE_2D, 0, TextureFormatToOpenGL(this->m_Format), this->m_Width, this->m_Height, 0, srgb ? GL_SRGB8 : TextureFormatToOpenGL(this->m_Format), GL_UNSIGNED_BYTE, this->m_ImageData);
				glGenerateMipmap(GL_TEXTURE_2D);

				glBindTexture(GL_TEXTURE_2D, 0);
			}
			stbi_image_free(this->m_ImageData);
		});
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		Renderer::Submit([this]() {
			glDeleteTextures(1, &this->m_RendererID);
		});
	}

	void OpenGLTexture2D::bind(unsigned int slot) const
	{
		Renderer::Submit([this, slot]() {
			glBindTextureUnit(slot, this->m_RendererID);
		});
	}
}