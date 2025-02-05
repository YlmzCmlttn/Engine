#pragma once

//#include "Core/Base.h"
#include "Renderer/Renderer.h"
#include "Renderer/Uniforms.h"

namespace Engine
{
	class Shader
	{
	public:
		virtual void bind() = 0;
		virtual void uploadUniformBuffer(const UniformBufferBase& uniformBuffer) = 0;
		static Shader* Create(const std::string& filepath);
	};

}