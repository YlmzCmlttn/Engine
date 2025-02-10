#pragma once

#include "Renderer/FrameBuffer.h"

namespace Engine {

    class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		virtual void resize(uint32_t width, uint32_t height, bool forceRecreate = false) override;

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void bindTexture(uint32_t slot = 0) const override;

		virtual RendererID getRendererID() const { return m_RendererID; }
		virtual RendererID getColorAttachmentRendererID() const { return m_ColorAttachment; }
		virtual RendererID getDepthAttachmentRendererID() const { return m_DepthAttachment; }

		virtual const FrameBufferSpecification& getSpecification() const override { return m_Specification; }
	private:
		FrameBufferSpecification m_Specification;
		RendererID m_RendererID = 0;
		RendererID m_ColorAttachment = 0, m_DepthAttachment = 0;
	};
}