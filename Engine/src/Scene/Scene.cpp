#include "Scene/Entity.h"
#include "Scene/Scene.h"
#include "Scene/Components.h"
#include "Scene/Systems.h"

namespace Engine {

    Scene::Scene(const std::string& name)
        : m_Name(name)
    {
        m_SceneEntity = m_Registry.create();
        m_Registry.emplace<RelationshipComponent>(m_SceneEntity);
        m_Registry.emplace<TransformComponent>(m_SceneEntity);
        m_Registry.on_construct<TransformComponent>().connect<&Systems::onTransformComponentConstruct>();
        m_Registry.on_update<TransformComponent>().connect<&Systems::onTransformComponentReplace>();
        m_Registry.on_construct<DirtyFlagComponent>().connect<&Systems::onDirtyFlagComponentConstruct>();
        m_Registry.on_update<DirtyFlagComponent>().connect<&Systems::onDirtyFlagComponentReplace>();
    }

    Scene::~Scene() {}

    void Scene::onAttach() {

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
        Engine::Renderer::Clear(0.1,0.1,0.1,1.0);
        auto view = m_Registry.view<TransformComponent, MeshComponent, MeshRendererComponent>();
        for (auto entity : view) {
            auto& transformComponent = view.get<TransformComponent>(entity);
            auto& meshComponent = view.get<MeshComponent>(entity);
            auto& meshRendererComponent = view.get<MeshRendererComponent>(entity);

            Engine::UniformBufferDeclaration<sizeof(glm::mat4), 1> ubo;
            ubo.push("u_MVP",projectionMatrix* viewMatrix* transformComponent.globalTransform);

            meshRendererComponent.material->bind();
            meshRendererComponent.material->getShader()->uploadUniformBuffer(ubo);
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