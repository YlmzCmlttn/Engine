#include "Platform/OpenGL/OpenGLTexture.h"

#include "Renderer/RendererAPI.h"
#include "Renderer/Renderer.h"

#include <glad/gl.h>

namespace Engine {

	static GLenum HazelToOpenGLTextureFormat(TextureFormat format)
	{
		switch (format)
		{
			case Engine::TextureFormat::RGB:     return GL_RGB;
			case Engine::TextureFormat::RGBA:    return GL_RGBA;
		}
		return 0;
	}

	OpenGLTexture2D::OpenGLTexture2D(TextureFormat format, unsigned int width, unsigned int height)
		: m_Format(format), m_Width(width), m_Height(height)
	{
		auto self = this;

		Renderer::Submit([self]() {
			glGenTextures(1, &self->m_RendererID);
			glBindTexture(GL_TEXTURE_2D, self->m_RendererID);
			glTexImage2D(GL_TEXTURE_2D, 0, HazelToOpenGLTextureFormat(self->m_Format), self->m_Width, self->m_Height, 0, HazelToOpenGLTextureFormat(self->m_Format), GL_UNSIGNED_BYTE, nullptr);
			glBindTexture(GL_TEXTURE_2D, 0);
		});
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		auto self = this;
		Renderer::Submit([self]() {
			glDeleteTextures(1, &self->m_RendererID);
		});
	}

}