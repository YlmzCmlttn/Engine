#pragma once

#include "Core/Layer.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/InspectorPanel.h"

class EditorLayer : public Engine::Layer {
public:
    EditorLayer();
    ~EditorLayer() override = default;

    void onAttach() override;
    void onDetach() override;

    void onUpdate([[maybe_unused]] Engine::Timestep ts) override;
    void onEvent([[maybe_unused]] Engine::Event& event) override;
    void onImGuiRender() override;

private:
    std::shared_ptr<SceneHierarchyPanel> m_SceneHierarchyPanel;
    std::shared_ptr<InspectorPanel> m_InspectorPanel;
    std::shared_ptr<Engine::Scene> m_Scene;
    std::shared_ptr<Engine::Mesh> m_Mesh;
    std::shared_ptr<Engine::Shader> m_Shader;
    std::shared_ptr<Engine::VertexBuffer> m_VB;
    std::shared_ptr<Engine::IndexBuffer> m_IB;
    std::shared_ptr<Engine::VertexArray> m_VA;
    std::shared_ptr<Engine::FrameBuffer> m_Framebuffer;
};
