#pragma once

#include "Renderer/RendererAPI.h"
#include "Core/Core.h"
#include <glm/glm.hpp>
namespace Engine {

    enum class FrameBufferFormat {
        None = 0,
        RGBA8 = 1,
        RGBA16F = 2,
        RGBA32F = 3,
        RGBA16 = 4,
        RGBAFloat = 5,
        RGBA32Float = 6,
    };


	struct FrameBufferSpecification
	{
		uint32_t width = 1280;
		uint32_t height = 720;
		glm::vec4 clearColor;
		FrameBufferFormat format;
		uint32_t samples = 1; // multisampling
		
		// SwapChainTarget = screen buffer (i.e. no framebuffer)
		bool swapChainTarget = false;
	};


    class FrameBuffer {
    public:
        virtual ~FrameBuffer() {}
		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void resize(uint32_t width, uint32_t height, bool forceRecreate = false) = 0;

		virtual void bindTexture(uint32_t slot = 0) const = 0;

		virtual RendererID getRendererID() const = 0;
		virtual RendererID getColorAttachmentRendererID() const = 0;
		virtual RendererID getDepthAttachmentRendererID() const = 0;

		virtual const FrameBufferSpecification& getSpecification() const = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
    
    protected:
        FrameBuffer() = default;

    };
}