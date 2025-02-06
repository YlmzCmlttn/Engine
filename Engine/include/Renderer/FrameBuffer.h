#pragma once

#include "Renderer/RendererAPI.h"
#include "Core/Core.h"

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

    class FrameBuffer {
    public:
        static Ref<FrameBuffer> Create(FrameBufferFormat format, unsigned int width, unsigned int height);

        virtual ~FrameBuffer() {}

        FrameBuffer(const FrameBuffer&) = delete;
        FrameBuffer& operator=(const FrameBuffer&) = delete;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void resize(unsigned int width, unsigned int height) = 0;

        virtual void bindTexture(unsigned int slot = 0) const = 0;

        virtual FrameBufferFormat getFormat() const = 0;
        virtual unsigned int getWidth() const = 0;
        virtual unsigned int getHeight() const = 0;

        virtual RendererID getRendererID() const = 0;
        virtual RendererID getColorAttachmentRendererID() const = 0;
        virtual RendererID getDepthAttachmentRendererID() const = 0;
    
    protected:
        FrameBuffer() = default;

    };
}