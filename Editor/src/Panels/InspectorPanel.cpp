#include "Panels/InspectorPanel.h"

#include <imgui.h>

InspectorPanel::InspectorPanel() {

}

InspectorPanel::~InspectorPanel() { 

}

void InspectorPanel::onImGuiRender() {
    ImGui::Begin("Inspector");
    ImGui::End();
}   