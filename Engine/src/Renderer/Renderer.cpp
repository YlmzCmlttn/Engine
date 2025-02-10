#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"

namespace Engine {

    Renderer* Renderer::s_Instance = new Renderer();
    RendererAPIType RendererAPI::s_CurrentRendererAPI = RendererAPIType::OpenGL;

    void Renderer::Init()
    {
        Submit([]() {   
            RendererAPI::Init();
        });
    }

    void Renderer::Clear() {
        Submit([]() {
            RendererAPI::Clear(0.0f, 0.0f, 0.0f, 1.0f);
        });
    }

    void Renderer::Clear(float r, float g, float b, float a) {
        Submit([r, g, b, a]() {
            RendererAPI::Clear(r, g, b, a);
        });
    }

    void Renderer::SetClearColor(float r, float g, float b, float a) {
        Submit([r, g, b, a]() {
            RendererAPI::SetClearColor(r, g, b, a);
        });
    }

    void Renderer::DrawIndexed(unsigned int indexCount,bool depthTest) {
        Submit([indexCount,depthTest]() {
            RendererAPI::DrawIndexed(indexCount,depthTest);
        });
    }

    void Renderer::waitAndRender() {
        m_CommandQueue.execute();
    }

}
