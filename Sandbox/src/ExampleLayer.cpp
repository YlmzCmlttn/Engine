#include "ExampleLayer.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"
#include "Core/Input.h"
#include "Core/Application.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Scene/Systems.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include <imgui.h>

ExampleLayer::ExampleLayer() : Layer("Example"){
    m_Scene = std::make_shared<Engine::Scene>("Example Scene");
}

void ExampleLayer::onAttach() {
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f
    };
    unsigned int indices[] = {
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
    
    auto cameraEntity = m_Scene->createEntity("Camera");    
    cameraEntity.addComponent<Engine::CameraComponent>();

    m_ParentEntity = m_Scene->createEntity("Parent");
    //m_ParentEntity.addComponent<Engine::TransformComponent>();
    
    m_ChildEntity = m_Scene->createEntity("Child");
    //m_ChildEntity.addComponent<Engine::TransformComponent>();
    //m_ChildEntity.setParent(m_ParentEntity);

    m_Child2Entity = m_Scene->createEntity("Child2");
    //m_Child2Entity.addComponent<Engine::TransformComponent>();
    //m_Child2Entity.setParent(m_ParentEntity);


}

void ExampleLayer::onDetach() {
}

void ExampleLayer::onUpdate([[maybe_unused]] Engine::Timestep ts) {
    using namespace Engine;
    UpdateTransforms(m_Scene->m_Registry);
    Renderer::Clear(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
    // Engine::UniformBufferDeclaration<sizeof(glm::vec4), 1> ubo;
    // ubo.push("u_Color", m_TriangleColor);
    // m_Shader->uploadUniformBuffer(ubo);

    // m_Shader->bind();
    //m_VA->bind();
    //Renderer::DrawIndexed(m_VA->getIndexBuffer()->getCount(),false);
    // Engine::Renderer::Submit([&]() {        
    //     Engine::RendererAPI::Clear(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
    // });



    if(Engine::Input::isKeyPressed(Engine::Key::D1)){
        //Make child1 a child of parent 
        auto& parentTransform = m_ParentEntity.getComponent<Engine::TransformComponent>();
        auto& childTransform = m_ChildEntity.getComponent<Engine::TransformComponent>();
        childTransform.setParent(parentTransform);
    }
    if(Engine::Input::isKeyPressed(Engine::Key::D2)){
        //Make child1 a child of parent 
        auto& parentTransform = m_ParentEntity.getComponent<Engine::TransformComponent>();
        auto& childTransform = m_ChildEntity.getComponent<Engine::TransformComponent>();
        childTransform.setParent(parentTransform,false);
    }
    if(Engine::Input::isKeyPressed(Engine::Key::D3)){
        //Make child1 a child of parent 
        auto& childTransform = m_ChildEntity.getComponent<Engine::TransformComponent>();
        childTransform.removeParent();
    }
    if(Engine::Input::isKeyPressed(Engine::Key::D4)){
        //Make child1 a child of parent 
        auto& childTransform = m_Child2Entity.getComponent<Engine::TransformComponent>();
        childTransform.removeParent();
    }

    if(Engine::Input::isKeyPressed(Engine::Key::A)){
        auto& parentTransform = m_ChildEntity.getComponent<Engine::TransformComponent>();
        parentTransform.localPosition = parentTransform.localPosition + glm::vec3(-0.1f, 0.0f, 0.0f);        
    }
    if(Engine::Input::isKeyPressed(Engine::Key::D)){
        auto& parentTransform = m_ChildEntity.getComponent<Engine::TransformComponent>();
        parentTransform.localPosition = parentTransform.localPosition + glm::vec3(0.1f, 0.0f, 0.0f);        
    }
    

    if(Engine::Input::isKeyPressed(Engine::Key::Left)){
        auto& parentTransform = m_ParentEntity.getComponent<Engine::TransformComponent>();
        parentTransform.localPosition = parentTransform.localPosition + glm::vec3(-0.1f, 0.0f, 0.0f);        
    }
    if(Engine::Input::isKeyPressed(Engine::Key::Right)){
        auto& parentTransform = m_ParentEntity.getComponent<Engine::TransformComponent>();
        parentTransform.localPosition = parentTransform.localPosition + glm::vec3(0.1f, 0.0f, 0.0f);        
    }
    if(Engine::Input::isKeyPressed(Engine::Key::Up)){
        auto& parentTransform = m_ParentEntity.getComponent<Engine::TransformComponent>();
        parentTransform.localPosition = parentTransform.localPosition + glm::vec3(0.0f, 0.1f, 0.0f);        
    }
    if(Engine::Input::isKeyPressed(Engine::Key::Down)){
        auto& parentTransform = m_ParentEntity.getComponent<Engine::TransformComponent>();
        parentTransform.localPosition = parentTransform.localPosition + glm::vec3(0.0f, -0.1f, 0.0f);        
    }
    //Print the transform of the parent
    std::cout << "Parent Transform: " << glm::to_string(static_cast<glm::vec3>(m_ParentEntity.getComponent<Engine::TransformComponent>().localPosition)) << std::endl;
    std::cout << "Child Transform: " << glm::to_string(static_cast<glm::vec3>(m_ChildEntity.getComponent<Engine::TransformComponent>().localPosition)) << std::endl;
    std::cout << "Child2 Transform: " << glm::to_string(static_cast<glm::vec3>(m_Child2Entity.getComponent<Engine::TransformComponent>().localPosition)) << std::endl;

    //Print world transform of the parent
    std::cout << "Parent World Transform: " << glm::to_string(m_ParentEntity.getComponent<Engine::TransformComponent>().worldTransform) << std::endl;
    std::cout << "Child World Transform: " << glm::to_string(m_ChildEntity.getComponent<Engine::TransformComponent>().worldTransform) << std::endl;
    std::cout << "Child2 World Transform: " << glm::to_string(m_Child2Entity.getComponent<Engine::TransformComponent>().worldTransform) << std::endl;

    
}

void ExampleLayer::onEvent([[maybe_unused]] Engine::Event& event) {
    
}

void ExampleLayer::onImGuiRender() {
    // static bool show_demo_window = true;
    // if (show_demo_window)
    //     ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Begin("GameLayer");
    ImGui::ColorEdit4("Clear Color", m_ClearColor);
    ImGui::ColorEdit4("Triangle Color", glm::value_ptr(m_TriangleColor));
    ImGui::End();
    // std::cout<<"ExampleLayer::onImGuiRender"<<std::endl;
    // ImGui::Begin("Example");
    // ImGui::Text("This is an example layer");
    // ImGui::End();
}
