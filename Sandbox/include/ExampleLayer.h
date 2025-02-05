#pragma once

#include "Core/Layer.h"

#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"



class ExampleLayer : public Engine::Layer {
public:
    ExampleLayer();
    ~ExampleLayer() override = default;

    void onAttach() override;
    void onDetach() override;

    void onUpdate([[maybe_unused]] Engine::Timestep ts) override;
    void onEvent([[maybe_unused]] Engine::Event& event) override;
    void onImGuiRender() override;

private:
    float m_ClearColor[4];
    std::unique_ptr<Engine::Shader> m_Shader;
    std::unique_ptr<Engine::VertexBuffer> m_VB;
    std::unique_ptr<Engine::IndexBuffer> m_IB;
};
