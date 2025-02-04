#include "Renderer/RendererAPI.h"
#include <glad/gl.h>
namespace Engine
{
    void RendererAPI::clear(float r, float g, float b, float a)
    {
        //std::cout<<"OpenGLRendererAPI::clear"<<std::endl;
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RendererAPI::setClearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }
} // namespace Engine
