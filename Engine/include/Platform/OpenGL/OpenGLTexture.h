#pragma once

#include <glad/gl.h>
#include "Renderer/Texture.h"

#include <glad/gl.h>
namespace Engine {

	GLenum TextureFormatToOpenGL(TextureFormat format);

	int CalculateMipMapCount(int width, int height);
}