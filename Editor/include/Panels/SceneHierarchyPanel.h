#pragma once

#include "Scene/Entity.h"
#include "Scene/Scene.h"
#include "Scene/Components.h"
class SceneHierarchyPanel {
public:
    SceneHierarchyPanel(const std::shared_ptr<Engine::Scene>& scene);
    ~SceneHierarchyPanel();

    void onImGuiRender();
    inline Engine::Entity getSelectedEntity() const { return m_SelectedEntity; }

private:
    inline void setSelectedEntity(Engine::Entity entity){ m_SelectedEntity = entity; }

    std::shared_ptr<Engine::Scene> m_Scene;
    Engine::Entity m_SelectedEntity;

    void drawEntityNode(Engine::Entity entity);
    Engine::Entity findEntityByTransform(Engine::TransformComponent* transform);
    void updateTransform(Engine::TransformComponent* transform);
};