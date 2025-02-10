#include "Platform/OpenGL/OpenGLFrameBuffer.h"
#include "Renderer/Renderer.h"
#include "Core/Assert.h"
#include <glad/gl.h>

namespace Engine {

    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
		: m_Specification(spec)
	{
		resize(spec.width, spec.height, true);
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		Renderer::Submit([this]() {
			glDeleteFramebuffers(1, &m_RendererID);
		});
	}

	void OpenGLFrameBuffer::resize(uint32_t width, uint32_t height, bool forceRecreate)
	{
		if (!forceRecreate && (m_Specification.width == width && m_Specification.height == height))
			return;

		m_Specification.width = width;
		m_Specification.height = height;
		Renderer::Submit([this]()
		{
			if (m_RendererID)
			{
				glDeleteFramebuffers(1, &m_RendererID);
				glDeleteTextures(1, &m_ColorAttachment);
				glDeleteTextures(1, &m_DepthAttachment);
			}			
			glGenFramebuffers(1, &m_RendererID);
			glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

			bool multisample = m_Specification.samples > 1;
			if (multisample)
			{
				glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_ColorAttachment);
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ColorAttachment);

				if (m_Specification.format == FrameBufferFormat::RGBA16F)
				{
					glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_Specification.samples, GL_RGBA16F, m_Specification.width, m_Specification.height, GL_FALSE);
				}
				else if (m_Specification.format == FrameBufferFormat::RGBA8)
				{
					glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_Specification.samples, GL_RGBA8, m_Specification.width, m_Specification.height, GL_FALSE);
				}
				// glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				// glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
			}
			else
			{
				glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
				glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);

				if (m_Specification.format == FrameBufferFormat::RGBA16F)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Specification.width, m_Specification.height, 0, GL_RGBA, GL_FLOAT, nullptr);
				}
				else if (m_Specification.format == FrameBufferFormat::RGBA8)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Specification.width, m_Specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
				}
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);
			}

			if (multisample)
			{
				glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_DepthAttachment);
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_DepthAttachment);
				glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_Specification.samples, GL_DEPTH24_STENCIL8, m_Specification.width, m_Specification.height, GL_FALSE);
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
			}
			else
			{
				glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
				glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
				glTexImage2D(
					GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.width, m_Specification.height, 0,
					GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
				);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
			}

			if (multisample)
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_ColorAttachment, 0);
			else
				glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_ColorAttachment, 0);

			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, m_DepthAttachment, 0);

			ENGINE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		});
	}

	void OpenGLFrameBuffer::bind() const
	{
		Renderer::Submit([=]() {
			glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
			glViewport(0, 0, m_Specification.width, m_Specification.height);
		});
	}

	void OpenGLFrameBuffer::unbind() const
	{
		Renderer::Submit([=]() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		});
	}

	void OpenGLFrameBuffer::bindTexture(uint32_t slot) const
	{
		Renderer::Submit([=]() {
			glBindTextureUnit(slot, m_ColorAttachment);
		});
	}
}