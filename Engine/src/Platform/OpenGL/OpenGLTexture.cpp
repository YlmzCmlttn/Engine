#include "Platform/OpenGL/OpenGLTexture.h"
#include "Renderer/Texture.h"
#include <glad/gl.h>

namespace Engine {

	GLenum TextureFormatToOpenGL(TextureFormat format)
	{
		switch (format)
		{
			case TextureFormat::RGB:     return GL_RGB;
			case TextureFormat::RGBA:    return GL_RGBA;
			// Add other cases as needed
			default: return GL_NONE;
		}
	}

	int CalculateMipMapCount(int width, int height)
	{
		int levels = 1;
		while ((width | height) >> levels) {
			levels++;
		}
		return levels;
	}
}