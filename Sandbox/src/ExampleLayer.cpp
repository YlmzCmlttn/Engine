#include "ExampleLayer.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"

#include <imgui.h>

ExampleLayer::ExampleLayer() : Layer("Example"),m_TriangleColor {1.0f, 0.0f, 1.0f, 1.0f} {}

void ExampleLayer::onAttach() {
    static float vertices[] = {
        -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
    };

    static unsigned int indices[] = {
        0, 1, 2
    };

    m_VB = std::unique_ptr<Engine::VertexBuffer>(Engine::VertexBuffer::Create());
    m_VB->setData(vertices, sizeof(vertices));

    m_IB = std::unique_ptr<Engine::IndexBuffer>(Engine::IndexBuffer::Create());
    m_IB->setData(indices, sizeof(indices));

    m_Shader.reset(Engine::Shader::Create("../assets/shaders/shader.glsl"));
}

void ExampleLayer::onDetach() {
}

void ExampleLayer::onUpdate([[maybe_unused]] Engine::Timestep ts) {
    using namespace Engine;
    Renderer::Clear(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
    Engine::UniformBufferDeclaration<sizeof(glm::vec4), 1> ubo;
    ubo.push("u_Color", m_TriangleColor);
    m_Shader->uploadUniformBuffer(ubo);

    m_Shader->bind();
    m_VB->bind();
    m_IB->bind();
    Renderer::DrawIndexed(3);
    // Engine::Renderer::Submit([&]() {        
    //     Engine::RendererAPI::Clear(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
    // });
}

void ExampleLayer::onEvent([[maybe_unused]] Engine::Event& event) {
    // Handle events if needed
}

void ExampleLayer::onImGuiRender() {
    // static bool show_demo_window = true;
    // if (show_demo_window)
    //     ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Begin("GameLayer");
    ImGui::ColorEdit4("Clear Color", m_ClearColor);
    ImGui::ColorEdit4("Triangle Color", glm::value_ptr(m_TriangleColor));
    ImGui::End();
    // std::cout<<"ExampleLayer::onImGuiRender"<<std::endl;
    // ImGui::Begin("Example");
    // ImGui::Text("This is an example layer");
    // ImGui::End();
}
