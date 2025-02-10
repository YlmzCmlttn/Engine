#include "Panels/SceneHierarchyPanel.h"

#include <imgui.h>

SceneHierarchyPanel::SceneHierarchyPanel() {

}

SceneHierarchyPanel::~SceneHierarchyPanel() {

}

void SceneHierarchyPanel::onImGuiRender() {
    ImGui::Begin("Scene Hierarchy");

    if(ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup("PanelContextMenu");
    }

    if(ImGui::BeginPopup("PanelContextMenu")) {
        if(ImGui::MenuItem("Create Empty Entity")) {
            
        }
        ImGui::EndPopup();
    }
    ImGui::End();
}