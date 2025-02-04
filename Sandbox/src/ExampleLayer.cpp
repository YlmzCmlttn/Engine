#include "ExampleLayer.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"

#include <imgui.h>

ExampleLayer::ExampleLayer() : Layer("Example") {}

void ExampleLayer::onAttach() {
    std::cout << "ExampleLayer attached" << std::endl;
}

void ExampleLayer::onDetach() {
    std::cout << "ExampleLayer detached" << std::endl;
}

void ExampleLayer::onUpdate([[maybe_unused]] Engine::Timestep ts) {
    std::cout<<"ExampleLayer updated"<<std::endl;
    Engine::Renderer::submit([&]() {
        std::cout<<"ExampleLayer::onUpdate submit"<<std::endl;
        Engine::RendererAPI::clear(0.2f, 0.3f, 0.8f, 1.0f);
    });
}

void ExampleLayer::onEvent([[maybe_unused]] Engine::Event& event) {
    // Handle events if needed
}

void ExampleLayer::onImGuiRender() {
    std::cout<<"ExampleLayer::onImGuiRender"<<std::endl;
    ImGui::Begin("Example");
    ImGui::Text("This is an example layer");
    ImGui::End();
}
