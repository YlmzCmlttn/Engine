#include "Panels/InspectorPanel.h"
#include "Core/Application.h"
#include <imgui.h>
#include <imgui_internal.h>
#include "Scene/Components.h"
#include "UI/EntityCommands.h"
#include "UI/UndoManager.h"
#include "Renderer/Material.h"

#include "glm/gtc/type_ptr.hpp"


static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f){
    bool resetted = false;
    ImGuiIO& io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text("%s", label.c_str());
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
    
    ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
    float lineHeight = ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
    //ImGui::Separator();

    //It may change later !!
    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth;
    bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "%s", name.c_str());
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
        ImGui::Text("UUID: %lu", uuid);
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

    DrawComponent<MeshComponent>("Mesh", entity, [entity](auto& component)
    {
        ImGui::Text("Mesh");
    });

    DrawComponent<MeshRendererComponent>("Mesh Renderer", entity, [entity](auto& component)
    {
        ImGui::Text("Mesh Renderer");
        
        ImGui::Text("Material");

        ImGui::Text("Albedo");
        
        Ref<Material> material = component.material;
        glm::vec3 albedo = material->get<glm::vec3>("Material.u_Albedo");
        ImGui::ColorEdit3("Albedo", glm::value_ptr(albedo));
        ImGui::Text("Metallic");
        float metallic = material->get<float>("Material.u_Metallic");
        ImGui::DragFloat("Metallic", &metallic, 0.01f, 0.0f, 1.0f);
        ImGui::Text("Roughness");
        float roughness = material->get<float>("Material.u_Roughness");
        ImGui::DragFloat("Roughness", &roughness, 0.01f, 0.0f, 1.0f);
        ImGui::Text("Ambient Occlusion");
        float ambientOcclusion = material->get<float>("Material.u_AmbientOcclusion");
        ImGui::DragFloat("Ambient Occlusion", &ambientOcclusion, 0.01f, 0.0f, 1.0f);

        material->set("Material.u_Albedo", albedo);
        material->set("Material.u_Metallic", metallic);
        material->set("Material.u_Roughness", roughness);
        material->set("Material.u_AmbientOcclusion", ambientOcclusion);


        ///Lights

        ImGui::Text("Lights");

        ImGui::Text("Light1 Position");
        glm::vec3 light1Position = material->get<glm::vec3>("Light.u_LightPosition[0]");
        DrawVec3Control("Light1 Position", light1Position);
        material->set("Light.u_LightPosition[0]", light1Position);

        ImGui::Text("Light1 Color");
        glm::vec3 light1Color = material->get<glm::vec3>("Light.u_LightColor[0]");
        ImGui::ColorEdit3("Light1 Color", glm::value_ptr(light1Color));
        ImGui::Text("Light1 Intensity");
        float light1Intensity = material->get<float>("Light.u_LightIntensity[0]");
        ImGui::DragFloat("Light1 Intensity", &light1Intensity, 0.01f, 0.0f, 1000.0f);
        material->set("Light.u_LightColor[0]", light1Color);
        material->set("Light.u_LightIntensity[0]", light1Intensity);

        ImGui::Text("Light2 Position");
        glm::vec3 light2Position = material->get<glm::vec3>("Light.u_LightPosition[1]");
        DrawVec3Control("Light2 Position", light2Position);
        material->set("Light.u_LightPosition[1]", light2Position);

        ImGui::Text("Light2 Color");
        glm::vec3 light2Color = material->get<glm::vec3>("Light.u_LightColor[1]");
        ImGui::ColorEdit3("Light2 Color", glm::value_ptr(light2Color));
        material->set("Light.u_LightColor[1]", light2Color);
        ImGui::Text("Light2 Intensity");
        float light2Intensity = material->get<float>("Light.u_LightIntensity[1]");
        ImGui::DragFloat("Light2 Intensity", &light2Intensity, 0.01f, 0.0f, 1000.0f);
        material->set("Light.u_LightIntensity[1]", light2Intensity);

        ImGui::Text("Light3 Position");
        glm::vec3 light3Position = material->get<glm::vec3>("Light.u_LightPosition[2]");
        DrawVec3Control("Light3 Position", light3Position);
        material->set("Light.u_LightPosition[2]", light3Position);

        ImGui::Text("Light3 Color");
        glm::vec3 light3Color = material->get<glm::vec3>("Light.u_LightColor[2]");
        ImGui::ColorEdit3("Light3 Color", glm::value_ptr(light3Color));
        material->set("Light.u_LightColor[2]", light3Color);
        ImGui::Text("Light3 Intensity");
        float light3Intensity = material->get<float>("Light.u_LightIntensity[2]");
        ImGui::DragFloat("Light3 Intensity", &light3Intensity, 0.01f, 0.0f, 1000.0f);
        material->set("Light.u_LightIntensity[2]", light3Intensity);

        ImGui::Text("Light4 Position");
        glm::vec3 light4Position = material->get<glm::vec3>("Light.u_LightPosition[3]");
        DrawVec3Control("Light4 Position", light4Position);
        material->set("Light.u_LightPosition[3]", light4Position);

        ImGui::Text("Light4 Color");
        glm::vec3 light4Color = material->get<glm::vec3>("Light.u_LightColor[3]");
        ImGui::ColorEdit3("Light4 Color", glm::value_ptr(light4Color));
        material->set("Light.u_LightColor[3]", light4Color);

        ImGui::Text("Light4 Intensity");
        float light4Intensity = material->get<float>("Light.u_LightIntensity[3]");
        ImGui::DragFloat("Light4 Intensity", &light4Intensity, 0.01f, 0.0f, 1000.0f);
        material->set("Light.u_LightIntensity[3]", light4Intensity);
        
        
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

    DrawComponent<CameraComponent>("Camera", entity, []([[maybe_unused]]auto& component)
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
