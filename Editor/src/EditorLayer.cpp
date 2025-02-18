#include "EditorLayer.h"
#include "Scene/Systems.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"

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

}

void EditorLayer::onAttach() {

    //Triangle
    float vertices_[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f
    };

    unsigned int indices_[] = {
        0, 1, 2,
        2, 3, 0,
        4, 5, 6,
        6, 7, 4,
        0, 3, 7,
        7, 4, 0,
        1, 5, 6,
        6, 2, 1,
        0, 1, 5,
        5, 4, 0,
        3, 2, 6,
        6, 7, 3
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
    spec.width = 1024;
    spec.height = 768;
    spec.format = Engine::FrameBufferFormat::RGBA8;
    //spec.samples = 1;
    //spec.swapChainTarget = false;
    //spec.clearColor = glm::vec4(0.0,0.0,1.0,1.0);
    m_Framebuffer = Engine::FrameBuffer::Create(spec);

    m_Scene = Engine::CreateRef<EditorScene>();
    m_SceneHierarchyPanel = Engine::CreateRef<SceneHierarchyPanel>(m_Scene);
    m_InspectorPanel = Engine::CreateRef<InspectorPanel>();
    m_ViewportPanel = Engine::CreateRef<ViewportPanel>(m_Framebuffer);
    //auto parentEntity = m_Scene->createEntity("Parent");
    for(uint i=0;i<2;i++){
        auto cameraEntity = m_Scene->createEntity("Child"+std::to_string(i));
        auto parentEntity = m_Scene->createEntity("Parent"+std::to_string(i));
        cameraEntity.setParent(parentEntity);
    }

    auto meshEntity = m_Scene->createEntity("Mesh");
    auto meshComponent = meshEntity.addComponent<Engine::MeshComponent>(m_Mesh);
    meshComponent.mesh = m_Mesh;

    
    m_Shader = Engine::Shader::CreateFromFile("Basic", "../assets/shaders/shader.glsl");


    //parentEntities[0].setParent(parentEntities[1]);
    //auto child0Entity = parentEntities[0].getChild(0);
    //parentEntities[1].setParent(child0Entity);

    // m_Scene->destroyEntity(parentEntities[2]);
    // m_Scene->destroyEntity(parentEntities[3]);
    


    //auto duplicatedEntity = m_Scene->duplicateEntity(parentEntity);
    //auto duplicatedEntity2 = m_Scene->duplicateEntity(parentEntity);

    //m_Scene->destroyEntity(duplicatedEntity2);
    //m_ParentEntity.addComponent<Engine::TransformComponent>();
    
    //auto childEntity = m_Scene->createEntity("Child");
    //m_ChildEntity.addComponent<Engine::TransformComponent>();
    //m_ChildEntity.setParent(m_ParentEntity);

    //auto child2Entity = m_Scene->createEntity("Child2");
    //child2Entity.getComponent<Engine::TransformComponent>().setParent(parentEntity.getComponent<Engine::TransformComponent>(),false);
}

void EditorLayer::onDetach() {
}

void EditorLayer::onUpdate([[maybe_unused]] Engine::Timestep ts) {
    m_Framebuffer->bind();

    Engine::Systems::UpdateTransforms(m_Scene);
    //Engine::Renderer::Clear(0.1,0.1,0.1,1.0);
    Engine::UniformBufferDeclaration<sizeof(glm::vec4), 1> ubo;
    ubo.push("u_Color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    m_Shader->uploadUniformBuffer(ubo);
    m_Shader->bind();
    m_Mesh->render();

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
