#include "Platform/OpenGL/OpenGLFrameBuffer.h"
#include "Renderer/Renderer.h"
#include <glad/gl.h>

namespace Engine {

    OpenGLFrameBuffer::OpenGLFrameBuffer(FrameBufferFormat format, unsigned int width, unsigned int height)
        : m_Format(format), m_Width(width), m_Height(height), m_RendererID(0), m_ColorAttachmentRendererID(0), m_DepthAttachmentRendererID(0)
    {
        resize(width, height);
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        if (m_RendererID) {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(1, &m_ColorAttachmentRendererID);
            glDeleteTextures(1, &m_DepthAttachmentRendererID);
        }
    }

    void OpenGLFrameBuffer::bind() const
    {
        Renderer::Submit([this]() {
            glBindFramebuffer(GL_FRAMEBUFFER, this->m_RendererID);
            glViewport(0, 0, this->m_Width, this->m_Height);
        });
    }

    void OpenGLFrameBuffer::unbind() const
    {
        Renderer::Submit([this]() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        });
    }

    void OpenGLFrameBuffer::resize(unsigned int width, unsigned int height)
    {
        if(m_Width == width && m_Height == height)
            return;

        m_Width = width;
        m_Height = height;

        Renderer::Submit([this]() {
            if(this->m_RendererID){
                glDeleteFramebuffers(1, &this->m_RendererID);
                glDeleteTextures(1, &this->m_ColorAttachmentRendererID);
                glDeleteTextures(1, &this->m_DepthAttachmentRendererID);
            }

            glGenFramebuffers(1, &this->m_RendererID);
            glBindFramebuffer(GL_FRAMEBUFFER, this->m_RendererID);

            glGenTextures(1, &this->m_ColorAttachmentRendererID);
            glBindTexture(GL_TEXTURE_2D, this->m_ColorAttachmentRendererID);

            if(this->m_Format == FrameBufferFormat::RGBA8){
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->m_Width, this->m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            }else if(this->m_Format == FrameBufferFormat::RGBA16F){
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, this->m_Width, this->m_Height, 0, GL_RGBA, GL_HALF_FLOAT, nullptr);
            }else if(this->m_Format == FrameBufferFormat::RGBA32F){
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, this->m_Width, this->m_Height, 0, GL_RGBA, GL_FLOAT, nullptr);
            }else if(this->m_Format == FrameBufferFormat::RGBA16){
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, this->m_Width, this->m_Height, 0, GL_RGBA, GL_UNSIGNED_SHORT, nullptr);
            }else if(this->m_Format == FrameBufferFormat::RGBAFloat){
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, this->m_Width, this->m_Height, 0, GL_RGBA, GL_FLOAT, nullptr);
            }else if(this->m_Format == FrameBufferFormat::RGBA32Float){
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, this->m_Width, this->m_Height, 0, GL_RGBA, GL_FLOAT, nullptr);
            }

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_ColorAttachmentRendererID, 0);

            glGenTextures(1, &this->m_DepthAttachmentRendererID);
            glBindTexture(GL_TEXTURE_2D, this->m_DepthAttachmentRendererID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, this->m_Width, this->m_Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, this->m_DepthAttachmentRendererID, 0);

            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout<<"Framebuffer is not complete!"<<std::endl;   

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        });
    }

    void OpenGLFrameBuffer::bindTexture(unsigned int slot) const
    {
        Renderer::Submit([this, slot]() {
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, this->m_ColorAttachmentRendererID);
        });
    }
}