#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"

namespace Engine {

    Renderer* Renderer::s_Instance = new Renderer();


    void Renderer::clear(float r, float g, float b, float a) {
        submit([r, g, b, a]() {
            RendererAPI::clear(r, g, b, a);
        });
    }

    void Renderer::setClearColor(float r, float g, float b, float a) {
        submit([r, g, b, a]() {
            std::cout << "Set clear color to (" << r << ", " << g << ", " << b << ", " << a << ")" << std::endl;
        });
    }

    void Renderer::waitAndRender() {
        m_CommandQueue.execute();
    }

}
