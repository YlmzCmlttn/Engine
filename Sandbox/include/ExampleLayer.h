#pragma once

#include "Core/Layer.h"

#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Scene/Scene.h"
#include "Renderer/Mesh.h"
#include "Scene/Entity.h"
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
    Engine::Entity m_ParentEntity;
    Engine::Entity m_ChildEntity;
    Engine::Entity m_Child2Entity;
    float m_ClearColor[4];
    glm::vec4 m_TriangleColor;
    std::shared_ptr<Engine::Mesh> m_Mesh;
    std::shared_ptr<Engine::Scene> m_Scene;
    std::shared_ptr<Engine::Shader> m_Shader;
    std::shared_ptr<Engine::VertexBuffer> m_VB;
    std::shared_ptr<Engine::IndexBuffer> m_IB;
    std::shared_ptr<Engine::VertexArray> m_VA;
};
