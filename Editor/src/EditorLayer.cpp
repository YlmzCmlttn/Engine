#include "EditorLayer.h"
#include "Scene/Systems.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <imgui.h>

#include "Loader/MeshLoader.h"
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
    
    //Get path from std::filesystem
    m_Shader = Engine::Shader::CreateFromFile("Basic", std::filesystem::current_path().string()+"/../assets/shaders/shader.glsl");
}

void generateCube(std::vector<Engine::Mesh::Vertex>& vertices, std::vector<unsigned int>& indices, float size = 1.0f) {
    float half = size / 2.0f;

    // Cube vertices (position, normal, and UV for each face)
    Engine::Mesh::Vertex cubeVertices[] = {
        // Front Face
        {glm::vec3(-half, -half,  half), glm::vec3(0,  0,  1),  glm::vec2(0, 0)},
        {glm::vec3( half, -half,  half), glm::vec3(0,  0,  1),  glm::vec2(1, 0)},
        {glm::vec3( half,  half,  half), glm::vec3(0,  0,  1),  glm::vec2(1, 1)},
        {glm::vec3(-half,  half,  half), glm::vec3(0,  0,  1),  glm::vec2(0, 1)},

        // Back Face
        {glm::vec3( half, -half, -half), glm::vec3(0,  0, -1),  glm::vec2(0, 0)},
        {glm::vec3(-half, -half, -half), glm::vec3(0,  0, -1),  glm::vec2(1, 0)},
        {glm::vec3(-half,  half, -half), glm::vec3(0,  0, -1),  glm::vec2(1, 1)},
        {glm::vec3( half,  half, -half), glm::vec3(0,  0, -1),  glm::vec2(0, 1)},

        // Left Face
        {glm::vec3(-half, -half, -half), glm::vec3(-1,  0,  0),  glm::vec2(0, 0)},  
        {glm::vec3(-half, -half,  half), glm::vec3(-1,  0,  0),  glm::vec2(1, 0)},  
        {glm::vec3(-half,  half,  half), glm::vec3(-1,  0,  0),  glm::vec2(1, 1)},  
        {glm::vec3(-half,  half, -half), glm::vec3(-1,  0,  0),  glm::vec2(0, 1)},  

        // Right Face
        {glm::vec3( half, -half,  half),  glm::vec3(1,  0,  0),  glm::vec2(0, 0)},  
        {glm::vec3( half, -half, -half),  glm::vec3(1,  0,  0),  glm::vec2(1, 0)},  
        {glm::vec3( half,  half, -half),  glm::vec3(1,  0,  0),  glm::vec2(1, 1)},  
        {glm::vec3( half,  half,  half),  glm::vec3(1,  0,  0),  glm::vec2(0, 1)},  

        // Top Face
        {glm::vec3(-half,  half,  half),  glm::vec3(0,  1,  0), glm::vec2(0, 0)},  
        {glm::vec3( half,  half,  half),  glm::vec3(0,  1,  0), glm::vec2(1, 0)},  
        {glm::vec3( half,  half, -half),  glm::vec3(0,  1,  0), glm::vec2(1, 1)},  
        {glm::vec3(-half,  half, -half),  glm::vec3(0,  1,  0), glm::vec2(0, 1)},  

        // Bottom Face
        {glm::vec3(-half, -half, -half),  glm::vec3(0, -1,  0), glm::vec2(0, 0)},  
        {glm::vec3( half, -half, -half),  glm::vec3(0, -1,  0), glm::vec2(1, 0)},  
        {glm::vec3( half, -half,  half),  glm::vec3(0, -1,  0), glm::vec2(1, 1)},  
        {glm::vec3(-half, -half,  half),  glm::vec3(0, -1,  0), glm::vec2(0, 1)}   
    };

    // Indices for each face (two triangles per face)
    unsigned int cubeIndices[] = {
        0, 1, 2,  2, 3, 0,    // Front
        4, 5, 6,  6, 7, 4,    // Back
        8, 9,10, 10,11, 8,    // Left
        12,13,14, 14,15,12,   // Right
        16,17,18, 18,19,16,   // Top
        20,21,22, 22,23,20    // Bottom
    };

    // Store vertices and indices
    vertices.assign(cubeVertices, cubeVertices + 24);
    indices.assign(cubeIndices, cubeIndices + 36);
}
void generateSphere(std::vector<Engine::Mesh::Vertex>& vertices, std::vector<unsigned int>& indices, float radius, int stacks, int slices) {
    for (int i = 0; i <= stacks; ++i) {
        float phi = M_PI * float(i) / float(stacks); // Latitude angle (0 to PI)

        for (int j = 0; j <= slices; ++j) {
            float theta = 2.0f * M_PI * float(j) / float(slices); // Longitude angle (0 to 2PI)

            // Compute vertex position
            float x = radius * sin(phi) * cos(theta);
            float y = radius * cos(phi);
            float z = radius * sin(phi) * sin(theta);

            // Normal is just the unit vector of position for a sphere
            float nx = x / radius;
            float ny = y / radius;
            float nz = z / radius;

            // UV mapping
            float u = float(j) / float(slices);
            float v = float(i) / float(stacks);

            // Store vertex
            vertices.push_back(Engine::Mesh::Vertex(glm::vec3(x, y, z), glm::vec3(nx, ny, nz), glm::vec2(u, v)));
        }
    }

    // Generate indices for triangle strips
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            int first = i * (slices + 1) + j;
            int second = first + slices + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
}



void EditorLayer::onAttach() {

    std::vector<Engine::Mesh::Vertex> vertices;
    std::vector<unsigned int> indices;
    generateSphere(vertices, indices, 1.0f, 64, 64);

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
    for(int i=0;i<2;i++){
        auto childEntity = m_Scene->createEntity("Child"+std::to_string(i));
        auto parentEntity = m_Scene->createEntity("Parent"+std::to_string(i));
        childEntity.setParent(parentEntity);
    }

    // m_Texture = Engine::Texture2D::Create(std::filesystem::current_path().string()+"/../assets/textures/test.png");

    m_MeshEntity = m_Scene->createEntity("Mesh");
    auto meshComponent = m_MeshEntity.addComponent<Engine::MeshComponent>(m_Mesh);
    meshComponent.mesh = m_Mesh;
    


    auto meshRendererComponent = m_MeshEntity.addComponent<Engine::MeshRendererComponent>(Engine::Material::Create(m_Shader));
    Renderer::Submit([meshRendererComponent,this]() {

        glm::vec3 lightPositions[] = {
            glm::vec3(-10.0f,  10.0f, 10.0f),
            glm::vec3( 10.0f,  10.0f, 10.0f),
            glm::vec3(-10.0f, -10.0f, 10.0f),
            glm::vec3( 10.0f, -10.0f, 10.0f),
        };
        
        //Set default values for the shader
        meshRendererComponent.material->set("Material.u_Albedo",glm::vec3(1.0,1.0,1.0));
        meshRendererComponent.material->set("Material.u_Metallic",0.0f);
        meshRendererComponent.material->set("Material.u_Roughness",0.5f);
        meshRendererComponent.material->set("Material.u_AmbientOcclusion",1.0f);

        //Set default values for the lights
        meshRendererComponent.material->set("Light.u_LightPosition[0]",lightPositions[0]);
        meshRendererComponent.material->set("Light.u_LightColor[0]",glm::vec3(1.0,1.0,1.0));
        meshRendererComponent.material->set("Light.u_LightIntensity[0]",300.0f);

        meshRendererComponent.material->set("Light.u_LightPosition[1]",lightPositions[1]);
        meshRendererComponent.material->set("Light.u_LightColor[1]",glm::vec3(1.0,1.0,1.0));
        meshRendererComponent.material->set("Light.u_LightIntensity[1]",300.0f);

        meshRendererComponent.material->set("Light.u_LightPosition[2]",lightPositions[2]);
        meshRendererComponent.material->set("Light.u_LightColor[2]",glm::vec3(1.0,1.0,1.0));
        meshRendererComponent.material->set("Light.u_LightIntensity[2]",300.0f);

        meshRendererComponent.material->set("Light.u_LightPosition[3]",lightPositions[3]);
        meshRendererComponent.material->set("Light.u_LightColor[3]",glm::vec3(1.0,1.0,1.0));
        meshRendererComponent.material->set("Light.u_LightIntensity[3]",300.0f);
        
    });

    //Engine::MeshLoader::GetInstance().LoadMeshToScene(std::filesystem::current_path().string()+"/../assets/models/backpack/Survival_BackPack_2.fbx",m_Scene);
    //Engine::MeshLoader::GetInstance().LoadMeshToScene(std::filesystem::current_path().string()+"/../assets/models/backpack/backpack.obj",m_Scene);
    //Engine::MeshLoader::GetInstance().LoadMeshToScene("../assets/models/m1911/m1911.fbx", m_Scene);
    //Engine::MeshLoader::GetInstance().LoadMeshToScene("../assets/models/Sphere1m.fbx", m_Scene);
    

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
