#include "ExampleLayer.h"

ExampleLayer::ExampleLayer() : Layer("Example") {}

void ExampleLayer::onAttach() {
    std::cout << "ExampleLayer attached" << std::endl;
}

void ExampleLayer::onDetach() {
    std::cout << "ExampleLayer detached" << std::endl;
}

void ExampleLayer::onUpdate(Engine::Timestep ts) {
}

void ExampleLayer::onEvent(Engine::Event& event) {
}

void ExampleLayer::onImGuiRender() {
}