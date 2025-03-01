#include "Renderer/Texture2D.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace Engine {

	Ref<Texture2D> Texture2D::Create(TextureFormat format, unsigned int width, unsigned int height)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None: return nullptr;
			case RendererAPIType::OpenGL: return CreateRef<OpenGLTexture2D>(format, width, height);
		}
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path,bool srgb)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None: return nullptr;
			case RendererAPIType::OpenGL: return CreateRef<OpenGLTexture2D>(path,srgb);
		}
		return nullptr;
	}

}