#include "Platform/OpenGL/OpenGLTextureCube.h"
#include "Renderer/RendererAPI.h"
#include "Renderer/Renderer.h"
#include "stb_image.h"
#include "Core/Assert.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include <glad/gl.h>

namespace Engine {
	OpenGLTextureCube::OpenGLTextureCube(const std::string& path)
		: m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(false);
		m_ImageData = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb);

		m_Width = width;
		m_Height = height;
		m_Format = TextureFormat::RGB;

		unsigned int faceWidth = m_Width / 4;
		unsigned int faceHeight = m_Height / 3;
		ENGINE_CORE_ASSERT(faceWidth == faceHeight, "Non-square faces!");

		std::array<unsigned char*, 6> faces;

		for (int i = 0; i < 6; i++) {
			faces[i] = new unsigned char[faceWidth * faceHeight * 3];
		}

		int faceIndex = 0;


		for (size_t i = 0; i < 4; i++)
		{
			for (size_t y = 0; y < faceHeight; y++)
			{
				size_t yOffset = y + faceHeight;
				for (size_t x = 0; x < faceWidth; x++)
				{
					size_t xOffset = x + i * faceWidth;
					faces[faceIndex][(x + y * faceWidth) * 3 + 0] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 0];
					faces[faceIndex][(x + y * faceWidth) * 3 + 1] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 1];
					faces[faceIndex][(x + y * faceWidth) * 3 + 2] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 2];
				}
			}
			faceIndex++;
		}

		for (size_t i = 0; i < 3; i++)
		{
			// Skip the middle one
			if (i == 1)
				continue;

			for (size_t y = 0; y < faceHeight; y++)
			{
				size_t yOffset = y + i * faceHeight;
				for (size_t x = 0; x < faceWidth; x++)
				{
					size_t xOffset = x + faceWidth;
					faces[faceIndex][(x + y * faceWidth) * 3 + 0] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 0];
					faces[faceIndex][(x + y * faceWidth) * 3 + 1] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 1];
					faces[faceIndex][(x + y * faceWidth) * 3 + 2] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 2];
				}
			}
			faceIndex++;
		}
		
		Renderer::Submit([this,faces,faceWidth,faceHeight]() {
			glGenTextures(1, &this->m_RendererID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_RendererID);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTextureParameterf(this->m_RendererID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);

			auto format = TextureFormatToOpenGL(this->m_Format);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[2]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[0]);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[4]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[5]);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[1]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[3]);

			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

			glBindTexture(GL_TEXTURE_2D, 0);

			for (size_t i = 0; i < faces.size(); i++)
				delete[] faces[i];

			stbi_image_free(this->m_ImageData);
		});

		
	}

	OpenGLTextureCube::~OpenGLTextureCube()
	{
		Renderer::Submit([this]() {
			glDeleteTextures(1, &this->m_RendererID);
		});
	}

	void OpenGLTextureCube::bind(unsigned int slot) const
	{
		Renderer::Submit([this,slot]() {
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_RendererID);
		});
	}
}