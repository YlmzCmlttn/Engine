#include "Panels/SceneHierarchyPanel.h"
#include "Core/Application.h"
#include <imgui.h>
#include <imgui_internal.h>
#include "UI/UndoManager.h"
#include "UI/DeleteEntityCommand.h"

SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<Engine::Scene>& scene) 
    : m_Scene(scene) {

}

SceneHierarchyPanel::~SceneHierarchyPanel() {

}

void SceneHierarchyPanel::onImGuiRender() {
    ImGui::Begin("Scene Hierarchy");


    // --- Undo/Redo Buttons ---
    if (ImGui::Button("Undo")) {
        Engine::Application::Submit([this]() {
            Engine::UndoManager::get().undo();
        });
    }
    ImGui::SameLine();
    if (ImGui::Button("Redo")) {
        Engine::Application::Submit([this]() {
            Engine::UndoManager::get().redo();
        });
    }

    if(m_Scene) {

        auto sceneEntity = m_Scene->getSceneEntity();
        auto relationship = sceneEntity.getComponent<Engine::RelationshipComponent>();
        auto curr = relationship.first;
        for(uint32_t i = 0; i < relationship.children; i++) {
            if(curr) {
                drawEntityNode(curr);
            }
            curr = curr.getComponent<Engine::RelationshipComponent>().next;
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
            //auto& droppedTransform = droppedEntity.getComponent<Engine::TransformComponent>();
            //droppedEntity.removeParent();

            Engine::Application::Submit([droppedEntity, this]() {
                Engine::Entity(droppedEntity).setParent(this->m_Scene->getSceneEntity());
            });
            /*
            droppedTransform.removeParent();
            */
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
    auto& relationship = entity.getComponent<Engine::RelationshipComponent>();

    // Setup tree node flags.
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    bool isSelected = (m_SelectedEntity == entity);
    if (isSelected)
        flags |= ImGuiTreeNodeFlags_Selected;
    
    // If the entity has no children, mark it as a leaf.

    if(relationship.children == 0)
        flags |= ImGuiTreeNodeFlags_Leaf;
    
    // Use the entity handle as a unique ID.
    bool nodeOpen = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.tag.c_str());
    
    // Handle selection.
    if (ImGui::IsItemClicked())
        m_SelectedEntity = entity;
    
    static bool s_DeleteEntityDialogRequested = false;
    
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Delete Entity")) {
            if (relationship.children == 0) {                
                Engine::Application::Submit([entity, this]() {
                    std::unique_ptr<Engine::Command> command = std::make_unique<Engine::DeleteEntityCommand>(entity);
                    Engine::UndoManager::get().executeCommand(command);
                });
            } else {
                // Set the flag to open the delete confirmation dialog.
                s_DeleteEntityDialogRequested = true;
            }
        }
        if (ImGui::MenuItem("Duplicate Entity")) {
            Engine::Application::Submit([entity, this]() {
                m_Scene->duplicateEntity(entity);
            });
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
            Engine::Application::Submit([entity, this]() {
                std::unique_ptr<Engine::Command> command = std::make_unique<Engine::DeleteEntityCommand>(entity, false);
                Engine::UndoManager::get().executeCommand(command);
            });
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("No", ImVec2(120, 0))) {
            Engine::Application::Submit([entity, this]() {
                std::unique_ptr<Engine::Command> command = std::make_unique<Engine::DeleteEntityCommand>(entity, true);
                Engine::UndoManager::get().executeCommand(command);
            });
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
                Engine::Application::Submit([this,droppedEntity, entity]() {
                    Engine::Entity(droppedEntity).setParent(entity);
                });
            }
        }
        ImGui::EndDragDropTarget();
    }


    // --- Additional Drop Target for Reordering:
    // We add a small invisible drop target below this node to allow reordering.
    // When an entity is dropped here, we update its ordering relative to 'entity'.
    //ImGui::Dummy(ImVec2(0.0f, 0.0f)); // A little spacing.
    ImGui::PushID("ReorderTarget");
    // ImGui::InvisibleButton("##ReorderDropTarget", ImVec2(ImGui::GetContentRegionAvail().x, 1.0f));
    // if (ImGui::BeginDragDropTarget()) {
    //     if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ENTITY")) {
    //         uint32_t payloadEntityID = *(const uint32_t*)payload->Data;
    //         Engine::Entity droppedEntity{ (entt::entity)payloadEntityID, m_Scene };
    //         // Only allow reordering if the dropped entity is not the same as the target.
    //         if (droppedEntity != entity) {
    //             // Submit a reorder task.
    //             Engine::Application::Submit([this, droppedEntity, entity]() {
    //                 Engine::Entity(entity).getComponent<Engine::TagComponent>().tag = "Reordered";
    //                 Engine::Entity(droppedEntity).getComponent<Engine::TagComponent>().tag = "Reordered_Dropped";
    //                 // Implement 'reorderEntity' to update the parent's child list order.
    //                 //m_Scene->reorderEntity(droppedEntity, entity);
    //             });
    //         }
    //     }
    //     ImGui::EndDragDropTarget();
    // }
    // After drawing your tree node for 'entity':
    ImVec2 itemMin = ImGui::GetItemRectMin();
    ImVec2 itemMax = ImGui::GetItemRectMax();

    // Define an overlapping drop target area.
    // 'overlap' controls how much larger the active area is compared to the visible item.
    float overlap = 1.0f; 
    ImRect dropTargetRect(
        itemMin.x,
        itemMin.y - overlap,  // extend upward
        itemMax.x,
        itemMin.y + overlap   // extend downward
    );

    // Optional: Draw the drop target rect for debugging (semi-transparent red)
    // ImDrawList* draw_list = ImGui::GetWindowDrawList();
    // draw_list->AddRectFilled(dropTargetRect.Min, dropTargetRect.Max, IM_COL32(255, 0, 0, 50));

    if (ImGui::BeginDragDropTargetCustom(dropTargetRect,ImGui::GetID(entity))) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERARCHY_ENTITY")) {
            uint32_t payloadEntityID = *(const uint32_t*)payload->Data;
            Engine::Entity droppedEntity{ (entt::entity)payloadEntityID, m_Scene };
            // Only allow reordering if the dropped entity is not the same as the target.
            if (droppedEntity != entity) {
                // Submit a reorder task. You should implement reorderEntity() to adjust your internal ordering.
                Engine::Application::Submit([this, droppedEntity, entity]() {
                    m_Scene->reorderEntity(droppedEntity, entity);
                });
            }
        }
        ImGui::EndDragDropTarget();
    }
    ImGui::PopID();

    // If the node is open, recursively draw its children.
    if (nodeOpen) {
        // For each child pointer, we need to find its corresponding entity.        
        auto curr = relationship.first;
        for(uint32_t i = 0; i < relationship.children; i++) {
            if(curr) {
                drawEntityNode(curr);
            }
            curr = curr.getComponent<Engine::RelationshipComponent>().next;
        }
        ImGui::TreePop();
    }

    ImGui::PopID();
}
