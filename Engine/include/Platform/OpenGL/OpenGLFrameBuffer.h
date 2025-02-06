#pragma once

#include "Renderer/FrameBuffer.h"

namespace Engine {

    class OpenGLFrameBuffer : public FrameBuffer {
    public:
        OpenGLFrameBuffer(FrameBufferFormat format, unsigned int width, unsigned int height);
        ~OpenGLFrameBuffer();

        void bind() const override;
        void unbind() const override;

        void resize(unsigned int width, unsigned int height) override;

        void bindTexture(unsigned int slot = 0) const override;

        inline FrameBufferFormat getFormat() const override { return m_Format; }
        inline unsigned int getWidth() const override { return m_Width; }
        inline unsigned int getHeight() const override { return m_Height; }

        inline RendererID getRendererID() const override { return m_RendererID; }
        inline RendererID getColorAttachmentRendererID() const override { return m_ColorAttachmentRendererID; }
        inline RendererID getDepthAttachmentRendererID() const override { return m_DepthAttachmentRendererID; }
        
    private:
        FrameBufferFormat m_Format;
        unsigned int m_Width;
        unsigned int m_Height;
        RendererID m_RendererID;
        RendererID m_ColorAttachmentRendererID;
        RendererID m_DepthAttachmentRendererID;
    };
}