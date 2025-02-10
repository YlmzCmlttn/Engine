#pragma once

#include "Core/Layer.h"

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
};
