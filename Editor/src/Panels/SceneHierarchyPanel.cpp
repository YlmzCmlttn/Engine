#include "Panels/SceneHierarchyPanel.h"

#include <imgui.h>

SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<Engine::Scene>& scene) 
    : m_Scene(scene) {

}

SceneHierarchyPanel::~SceneHierarchyPanel() {

}

void SceneHierarchyPanel::onImGuiRender() {
    ImGui::Begin("Scene Hierarchy");

    if(m_Scene) {
        auto view = m_Scene->getRegistry().view<Engine::TransformComponent, Engine::TagComponent>();
        for (auto entity : view) {
            auto& transform = view.get<Engine::TransformComponent>(entity);
            // Only draw root entities (those without a parent).
            if (transform.parent == nullptr)
                drawEntityNode(Engine::Entity(entity, m_Scene));
        }       
    }

    // Create a drop target on empty background.
    // Get the available region and create an invisible button covering it.
    ImGui::InvisibleButton("##EmptyDropTarget", ImGui::GetContentRegionAvail());

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ENTITY")) {
            // Retrieve the entity handle from the payload.
            uint32_t payloadEntityID = *(const uint32_t*)payload->Data;
            Engine::Entity droppedEntity{ (entt::entity)payloadEntityID, m_Scene };

            // Remove the parent for this entity.
            auto& droppedTransform = droppedEntity.getComponent<Engine::TransformComponent>();
            droppedTransform.removeParent();
        }
        ImGui::EndDragDropTarget();
    }

    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Empty Entity"))
        {
            
        }
        if (ImGui::MenuItem("Mesh"))
        {
            
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Directional Light"))
        {
            
        }
        if (ImGui::MenuItem("Sky Light"))
        {
        }
		ImGui::EndPopup();
    }
   
    ImGui::End();
}
void SceneHierarchyPanel::drawEntityNode(Engine::Entity entity) {
    ImGui::PushID((int)(uint32_t)entity);
    // Get the tag (display name) and transform.
    auto& tag = entity.getComponent<Engine::TagComponent>();
    auto& transform = entity.getComponent<Engine::TransformComponent>();

    // Setup tree node flags.
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    bool isSelected = (m_SelectedEntity == entity);
    if (isSelected)
        flags |= ImGuiTreeNodeFlags_Selected;
    
    // If the entity has no children, mark it as a leaf.
    if (transform.children.empty())
        flags |= ImGuiTreeNodeFlags_Leaf;

    // Use the entity handle as a unique ID.
    bool nodeOpen = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.tag.c_str());
    
    // Handle selection.
    if (ImGui::IsItemClicked())
        m_SelectedEntity = entity;
    
    static bool s_DeleteEntityDialogRequested = false;

    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Delete Entity")) {
            if (transform.children.empty()) {
                m_Scene->destroyEntity(entity);
            } else {
                // Set the flag to open the delete confirmation dialog.
                s_DeleteEntityDialogRequested = true;
            }
        }
        if (ImGui::MenuItem("Duplicate Entity")) {
            m_Scene->duplicateEntity(entity);
        }
        ImGui::EndPopup();
    }

    // After the context menu closes, if deletion is requested, open the popup.
    if (s_DeleteEntityDialogRequested) {
        ImGui::OpenPopup("Delete Entity Confirmation");
        s_DeleteEntityDialogRequested = false; // Reset the flag.
    }

    if (ImGui::BeginPopupModal("Delete Entity Confirmation", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Do you want to delete all children of this entity?");
        ImGui::Separator();

        if (ImGui::Button("Yes", ImVec2(120, 0))) {
            m_Scene->destroyEntity(entity, false);  // Delete entity and its children.
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("No", ImVec2(120, 0))) {
            m_Scene->destroyEntity(entity, true);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    // --- Drag & Drop Source: allow this node to be dragged.
    if (ImGui::BeginDragDropSource()) {
        uint32_t entityID = (uint32_t)entity;
        ImGui::SetDragDropPayload("SCENE_HIERARCHY_ENTITY", &entityID, sizeof(uint32_t));
        ImGui::Text(tag.tag.c_str());
        ImGui::EndDragDropSource();
    }

    // --- Drag & Drop Target: allow other entities to be dropped onto this node.
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ENTITY")) {
            // Retrieve the dropped entity's handle.
            uint32_t payloadEntityID = *(const uint32_t*)payload->Data;
            Engine::Entity droppedEntity{ (entt::entity)payloadEntityID, m_Scene};

            // Avoid self parenting.
            if (droppedEntity != entity) {
                // Set the parent of the dropped entity to this entity.
                // Here, passing 'false' means the child's local transform remains unchanged.
                auto& droppedTransform = droppedEntity.getComponent<Engine::TransformComponent>();
                droppedTransform.setParent(entity.getComponent<Engine::TransformComponent>(), false);
            }
        }
        ImGui::EndDragDropTarget();
    }

    // If the node is open, recursively draw its children.
    if (nodeOpen) {
        // For each child pointer, we need to find its corresponding entity.
        for (auto childTransform : transform.children) {
            Engine::Entity childEntity = m_Scene->findEntityByTransformComponent(*childTransform);
            
            if (childEntity){
                drawEntityNode(childEntity);

            }
        }
        ImGui::TreePop();
    }
    ImGui::PopID();
}
