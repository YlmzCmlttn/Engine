#include "EditorLayer.h"
#include "Scene/Systems.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui.h>

static void DockSpaceBegin(){
     static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();

    ImGuiStyle& style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;


    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    style.WindowMinSize.x = minWinSizeX;
}

void DockSpaceEnd(){
    ImGui::End();
}

EditorLayer::EditorLayer() : Layer("Editor")
{

    m_Shader = Engine::Shader::CreateFromFile("Basic", "../assets/shaders/shader.glsl");

}

void EditorLayer::onAttach() {

    //Triangle
    float vertices_[] = {
    // Front face
    -0.5f, -0.5f,  0.5f,  
     0.5f, -0.5f,  0.5f,  
     0.5f,  0.5f,  0.5f,  
    -0.5f,  0.5f,  0.5f,  

    // Back face
    -0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f,  
     0.5f,  0.5f, -0.5f,  
    -0.5f,  0.5f, -0.5f,  
};
unsigned int indices_[] = {
    // Front face
    0, 1, 2,  2, 3, 0,  
    // Back face
    4, 5, 6,  6, 7, 4,  
    // Left face
    4, 0, 3,  3, 7, 4,  
    // Right face
    1, 5, 6,  6, 2, 1,  
    // Top face
    3, 2, 6,  6, 7, 3,  
    // Bottom face
    4, 5, 1,  1, 0, 4   
};


    std::vector<Engine::Mesh::Vertex> vertices;
    for(uint i=0;i<24;i=i+3){
        vertices.push_back(Engine::Mesh::Vertex(glm::vec3(vertices_[i],vertices_[i+1],vertices_[i+2])));
    }
    std::vector<uint32_t> indices;
    for(uint i=0;i<36;i++){
        indices.push_back(indices_[i]);
    }

    m_Mesh = Engine::CreateRef<Engine::Mesh>();
    m_Mesh->setVertices(vertices);
    m_Mesh->setIndices(indices);
    m_Mesh->uploadToGPU();


    Engine::FrameBufferSpecification spec;
    spec.width = 400;
    spec.height = 400;
    spec.format = Engine::FrameBufferFormat::RGBA8;
    //spec.samples = 1;
    //spec.swapChainTarget = false;
    //spec.clearColor = glm::vec4(0.0,0.0,1.0,1.0);
    m_Framebuffer = Engine::FrameBuffer::Create(spec);

    m_Scene = Engine::CreateRef<EditorScene>();
    m_Scene->onAttach();
    m_SceneHierarchyPanel = Engine::CreateRef<SceneHierarchyPanel>(m_Scene);
    m_InspectorPanel = Engine::CreateRef<InspectorPanel>();
    m_ViewportPanel = Engine::CreateRef<ViewportPanel>(m_Framebuffer);
    //auto parentEntity = m_Scene->createEntity("Parent");
    for(uint i=0;i<2;i++){
        auto childEntity = m_Scene->createEntity("Child"+std::to_string(i));
        auto parentEntity = m_Scene->createEntity("Parent"+std::to_string(i));
        childEntity.setParent(parentEntity);
    }

    m_MeshEntity = m_Scene->createEntity("Mesh");
    auto meshComponent = m_MeshEntity.addComponent<Engine::MeshComponent>(m_Mesh);
    meshComponent.mesh = m_Mesh;

    
    auto meshRendererComponent = m_MeshEntity.addComponent<Engine::MeshRendererComponent>(Engine::Material::Create(m_Shader));

    m_ViewportPanel->setActiveScene(m_Scene);
}

void EditorLayer::onDetach() {
}

void EditorLayer::onUpdate([[maybe_unused]] Engine::Timestep ts) {    
    m_Scene->onUpdate(ts);
    m_Framebuffer->bind();
    m_Scene->onRender(ts);
    m_Framebuffer->unbind();   
}

void EditorLayer::onEvent([[maybe_unused]] Engine::Event& event) {
    // Handle events if needed
    m_Scene->onEvent(event);
}

void EditorLayer::onImGuiRender() {

    DockSpaceBegin();

    m_ViewportPanel->onImGuiRender();
    m_SceneHierarchyPanel->onImGuiRender();
    m_InspectorPanel->setEntity(m_SceneHierarchyPanel->getSelectedEntity());
    m_InspectorPanel->onImGuiRender();

    DockSpaceEnd();
}
