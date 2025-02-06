#include "Renderer/RendererAPI.h"
#include "Core/Log.h"
#include <glad/gl.h>
namespace Engine
{

    static void OpenGLLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
			ENGINE_CORE_ERROR("{0}", message);
	}
    
    void RendererAPI::Init()
    {

        glDebugMessageCallback(OpenGLLogMessage, nullptr);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        unsigned int vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

        glEnable(GL_DEPTH_TEST);;
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        glFrontFace(GL_CCW);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        auto& caps = RendererAPI::GetCapabilities();
        caps.Vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        caps.Renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        caps.Version = reinterpret_cast<const char*>(glGetString(GL_VERSION)); 

        glGetIntegerv(GL_MAX_SAMPLES, &caps.MaxSamples);
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &caps.MaxAnisotropy);
        //glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &caps.MaxCombinedTextureUnits);
        //glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &caps.MaxVertexAttributes);
        //glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &caps.MaxVertexUniformVectors);
        //glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &caps.MaxFragmentUniformVectors);
    }
    void RendererAPI::Shutdown()
    {

    }
    void RendererAPI::Clear(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RendererAPI::SetClearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }

    void RendererAPI::DrawIndexed(unsigned int indexCount, bool depthTesting)
    {
        if(depthTesting)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);

        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    }
} // namespace Engine
