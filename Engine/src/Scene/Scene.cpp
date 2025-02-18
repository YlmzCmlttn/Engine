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

    void Scene::onUpdate(Timestep ts) {
        
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
        return Systems::CreateEntity(shared_from_this(), name);
    }

    Entity Scene::duplicateEntity(Entity entity) {
        return Systems::DuplicateEntity(entity);
    }

    void Scene::destroyEntity(Entity entity, bool keepChildren) {
        Systems::DestroyEntity(entity, keepChildren);
    }    
}