#include "Scene/Entity.h"
#include "Scene/Scene.h"
#include "Scene/Components.h"
#include "Scene/Systems.h"
#include <glm/gtc/type_ptr.hpp>
namespace Engine {

    Scene::Scene(const std::string& name)
        : m_Name(name)
    {
        m_SceneEntity = m_Registry.create();
        m_Registry.emplace<RelationshipComponent>(m_SceneEntity);
        m_Registry.emplace<TransformComponent>(m_SceneEntity);
        m_Registry.emplace<IdComponent>(m_SceneEntity);
        m_Registry.on_construct<TransformComponent>().connect<&Systems::onTransformComponentConstruct>();
        m_Registry.on_update<TransformComponent>().connect<&Systems::onTransformComponentReplace>();
        m_Registry.on_construct<DirtyFlagComponent>().connect<&Systems::onDirtyFlagComponentConstruct>();
        m_Registry.on_update<DirtyFlagComponent>().connect<&Systems::onDirtyFlagComponentReplace>();
        
    }

    Scene::~Scene() {}

    void Scene::onAttach() {
        IdComponent& idComponent = m_Registry.get<IdComponent>(m_SceneEntity);
        std::cout << "Scene Entity ID: " << idComponent.id << std::endl;
        m_EntityMap[idComponent.id] = Entity(m_SceneEntity, shared_from_this());
    }

    void Scene::onUpdate(Timestep ts) {
        Engine::Systems::UpdateTransforms(shared_from_this());
    }
    void Scene::onRender(Timestep ts) {
        auto mainCamera = getPrimaryCameraEntity();
        if(mainCamera){
            auto& cameraComponent = mainCamera.getComponent<CameraComponent>();
            auto& transformComponent = mainCamera.getComponent<TransformComponent>();
            auto& sceneCamera = cameraComponent.camera;
            onRender(ts,glm::inverse(transformComponent.globalTransform),sceneCamera.getProjectionMatrix());
        }
    }
    void Scene::onRender(Timestep ts, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
        //Get Mesh Components.
        static int count = 99;
        Engine::Renderer::Clear(0.1,0.1,0.1,1.0);
        auto view = m_Registry.view<TransformComponent, MeshComponent, MeshRendererComponent>();
        for (auto entity : view) {
            auto& transformComponent = view.get<TransformComponent>(entity);
            auto& meshComponent = view.get<MeshComponent>(entity);
            auto& meshRendererComponent = view.get<MeshRendererComponent>(entity);
            
            //auto mvp = projectionMatrix* viewMatrix* transformComponent.globalTransform;
            // struct Transforms{
            //     glm::mat4 u_M;
            //     glm::mat4 u_V;
            //     glm::mat4 u_P;
            // };
            // Transforms transforms;
            // transforms.u_M = transformComponent.globalTransform;
            // transforms.u_V = viewMatrix;
            // transforms.u_P = projectionMatrix;
            // meshRendererComponent.material->set("Transforms", transforms);
            count++;
            if(count == 100){
                count = 0;
                meshRendererComponent.material->set("Transforms.u_M",transformComponent.globalTransform);
            }
            meshRendererComponent.material->set("Transforms.u_V",viewMatrix);
            meshRendererComponent.material->set("Transforms.u_P",projectionMatrix);
            meshRendererComponent.material->bind();

            //meshRendererComponent.material->getShader()->setUniformBuffer("Transforms", glm::value_ptr(mvp), sizeof(glm::mat4));     
            //meshRendererComponent.material->getShader()->setUniform("color.u_Color",glm::vec4(1.0,1.0,0.0,1.0));
            meshComponent.mesh->render();
        }
        
    }
    void Scene::onEvent(Event& e) {

    }

    Entity Scene::getSceneEntity() {
        return Entity(m_SceneEntity, shared_from_this());
    }

    Entity Scene::getPrimaryCameraEntity() {
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view) {
            const auto& cameraComponent = view.get<CameraComponent>(entity);
            if (cameraComponent.primary) return Entity(entity, shared_from_this());
        }
        ENGINE_CORE_ERROR("No primary camera found");
        return Entity();
    }

    // Entity Scene::findEntityByTransformComponent(const TransformComponent& transform) {
    //     return Systems::FindEntityByTransformComponent(shared_from_this(), transform);
    // }

    void Scene::reorderEntity(Entity entity,Entity next) {
        Systems::ReorderEntity(entity,next);
    }
    
    Entity Scene::createEntity(const std::string& name) {
        Entity entity = Systems::CreateEntity(shared_from_this(), name);
        return entity;
    }

    Entity Scene::createEntityWithUUID(const std::string& name, const UUID& uuid){
        ENGINE_CORE_ASSERT(m_EntityMap.find(uuid) == m_EntityMap.end(),"Entity with UUID already exists");
        Entity entity = Systems::CreateEntityWithUUID(shared_from_this(), name, uuid);
        return entity;
    }

    Entity Scene::getEntityByUUID(const UUID& uuid) const {
        auto it = m_EntityMap.find(uuid);
        if (it != m_EntityMap.end()) {
            return it->second;
        }else{
            std::cout << "Entity with UUID:" << uuid << " not found" << std::endl;
            return Entity();
        }
    }

    Entity Scene::duplicateEntity(Entity entity) {
        return Systems::DuplicateEntity(entity);    
    }

    void Scene::destroyEntity(Entity entity, bool keepChildren) {
        Systems::DestroyEntity(entity, keepChildren);
    }    

    void Scene::onViewportResize(uint32_t width, uint32_t height) {
        m_ViewportWidth = width;
        m_ViewportHeight = height;
    }
}