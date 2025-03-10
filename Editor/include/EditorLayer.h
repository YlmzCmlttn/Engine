#pragma once

#include "Core/Layer.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/ViewportPanel.h"
#include "EditorScene/EditorScene.h"
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
    std::shared_ptr<ViewportPanel> m_ViewportPanel;
    std::shared_ptr<EditorScene> m_Scene;
    std::shared_ptr<Engine::Mesh> m_Mesh;
    std::shared_ptr<Engine::Material> m_Material;
    std::shared_ptr<Engine::Shader> m_Shader;
    std::shared_ptr<Engine::VertexBuffer> m_VB;
    Engine::Entity m_MeshEntity;
    std::shared_ptr<Engine::IndexBuffer> m_IB;
    std::shared_ptr<Engine::VertexArray> m_VA;
    std::shared_ptr<Engine::FrameBuffer> m_Framebuffer;
    std::shared_ptr<Engine::Texture2D> m_Texture;
};
