#pragma once
#include "Renderer/Texture.h"

namespace Engine {

	class Texture2D : public Texture
	{
	public:
		static Texture2D* Create(TextureFormat format, unsigned int width, unsigned int height);
		static Texture2D* Create(const std::string& path,bool srgb  = false);
	};

}