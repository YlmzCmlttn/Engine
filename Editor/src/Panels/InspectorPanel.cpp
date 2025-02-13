#include "Panels/InspectorPanel.h"

#include <imgui.h>

#include "Scene/Components.h"

template<typename T, typename UIFunction>
static void DrawComponent(const std::string& name, Engine::Entity entity, UIFunction uiFunction)
{
    using namespace Engine;
    if (!entity.hasComponent<T>())
        return;
    
    T& component = entity.getComponent<T>();
    ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
    float lineHeight = ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
    ImGui::Separator();

    //It may change later !!
    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth;
    bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
    ImGui::PopStyleVar();
    

    bool removeComponent = false;
    if(!(std::is_same<T,TransformComponent>::value)){
        ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
        
        if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight })){
            ImGui::OpenPopup("ComponentSettings");
        }

        if (ImGui::BeginPopup("ComponentSettings")){
            if (ImGui::MenuItem("Remove component")){
                removeComponent = true;
            }
            ImGui::EndPopup();
        }
    }

    if(open){
        uiFunction(entity);
        ImGui::TreePop();
    }

    if(removeComponent){
        entity.removeComponent<T>();
    }
}   

static void DrawEntity(Engine::Entity entity){
    using namespace Engine;
    DrawComponent<TagComponent>("Tag", entity, [](auto& component)
    {
        ImGui::Text("Tag");
    });

    DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
    {
        ImGui::Text("Translation");
        ImGui::Text("Rotation");
        ImGui::Text("Scale");
    });

    DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
    {
        ImGui::Text("Camera");
    });
}

InspectorPanel::InspectorPanel() {

}

InspectorPanel::~InspectorPanel() { 

}

void InspectorPanel::onImGuiRender() {
    ImGui::Begin("Inspector");
    if(m_Entity){
        DrawEntity(m_Entity);
    }
    ImGui::End();
}   
