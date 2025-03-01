#pragma once
#include "Renderer/Texture.h"
#include "Core/Core.h"

namespace Engine {

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(TextureFormat format, unsigned int width, unsigned int height);
		static Ref<Texture2D> Create(const std::string& path,bool srgb  = false);
	};

}