
#include "Panels/InspectorPanel.h"
#include "Core/Application.h"
#include <imgui.h>
#include <imgui_internal.h>
#include "Scene/Components.h"
#include "UI/EntityCommands.h"
#include "UI/UndoManager.h"



static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f){
    bool resetted = false;
    ImGuiIO& io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text(label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

    float lineHeight = ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.4f, 0.4f, 0.5f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.5f, 0.5f, 0.6f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.4f, 0.4f, 0.5f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize)){
        values.x = resetValue;
        resetted = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.3f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.4f, 0.4f, 0.5f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.5f, 0.5f, 0.6f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.4f, 0.4f, 0.5f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize)){
        values.y = resetValue;
        resetted = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.3f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.4f, 0.4f, 0.5f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.5f, 0.5f, 0.6f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.4f, 0.4f, 0.5f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize)){
        values.z = resetValue;
        resetted = true;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.3f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();
    return resetted;
}

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
    //ImGui::Separator();

    //It may change later !!
    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth;
    bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
    ImGui::PopStyleVar();
    

    bool removeComponent = false;
    if(!(std::is_same<T,TransformComponent>::value)){
        ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
        
        if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight })){
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
        uiFunction(entity.getComponent<T>());
        ImGui::TreePop();
    }

    if(removeComponent){
        entity.removeComponent<T>();
    }
}   

static void DrawEntity(Engine::Entity entity){
    using namespace Engine;
    DrawComponent<IdComponent>("UUID", entity, [entity](auto& component)
    {
        uint64_t uuid = component.id;
        ImGui::Text("UUID: %llu", uuid);
    });

    DrawComponent<TagComponent>("Tag", entity, [entity](auto& component)
    {
        ImGui::Text("Tag");
        std::string currentTag = component.tag;
        char buffer[256];
        strcpy(buffer, currentTag.c_str());
        if(ImGui::InputText("##Tag", buffer, sizeof(buffer))){
            Application::Submit([entity, buffer]() {
                std::unique_ptr<Command> command = std::make_unique<EntityTagChangeCommand>(Entity(entity), std::string(buffer));
                UndoManager::get().executeCommand(command);
            });
        }
    });

    DrawComponent<TransformComponent>("Transform", entity, [entity](auto& component)
    {
        static bool oldValueStored = false;
        static glm::vec3 oldTranslation;
        static glm::vec3 oldRotation;
        static glm::vec3 oldScale;
        if(!oldValueStored){
            oldTranslation = component.localPosition;
            oldRotation = glm::degrees(component.localRotation);
            oldScale = component.localScale;
            oldValueStored = true;
        }

        glm::vec3 translation = component.localPosition;
        glm::vec3 rotation = glm::degrees(component.localRotation);
        //Put positive values
        if(rotation.x < 0){
            rotation.x += 360.0f;
        }
        if(rotation.y < 0){
            rotation.y += 360.0f;
        }
        if(rotation.z < 0){
            rotation.z += 360.0f;
        }
        rotation.x = fmod(rotation.x, 360.0f);
        rotation.y = fmod(rotation.y, 360.0f);
        rotation.z = fmod(rotation.z, 360.0f);
        glm::vec3 scale = component.localScale;
        ImGui::BeginGroup();
        bool resetted = false;
        if(DrawVec3Control("Translation", translation) || DrawVec3Control("Rotation", rotation) || DrawVec3Control("Scale", scale,1.0f)){
            resetted = true;
        }
        ImGui::EndGroup();


        if(translation != component.localPosition || rotation != component.localRotation || scale != component.localScale){
            Application::Submit([entity, translation, rotation, scale]() {
                Entity(entity).setTransform(translation, glm::radians(rotation), scale);
            });
        }
        if(ImGui::IsItemDeactivatedAfterEdit() || resetted){
            oldValueStored = false;
            Application::Submit([entity, translation, rotation, scale]() {
                //Entity(entity).setTransform(translation, glm::radians(rotation), scale);
                std::unique_ptr<Command> command = std::make_unique<EntityTransformChangeCommand>(Entity(entity), translation, glm::radians(rotation), scale);
                UndoManager::get().executeCommand(command);
            });            
        }


        glm::vec3 global_translation = component.position;
        glm::vec3 global_rotation = glm::degrees(component.rotation);
        //Put positive values
        if(global_rotation.x < 0){
            global_rotation.x += 360.0f;
        }
        if(global_rotation.y < 0){
            global_rotation.y += 360.0f;
        }
        if(global_rotation.z < 0){
            global_rotation.z += 360.0f;
        }
        global_rotation.x = fmod(global_rotation.x, 360.0f);
        global_rotation.y = fmod(global_rotation.y, 360.0f);
        global_rotation.z = fmod(global_rotation.z, 360.0f);
        glm::vec3 global_scale = component.scale;
        if(DrawVec3Control("Global Translation", global_translation) || DrawVec3Control("Global Rotation", global_rotation) || DrawVec3Control("Global Scale", global_scale)){
            resetted = true;
        }
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
