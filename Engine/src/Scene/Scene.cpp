#include "Scene/Scene.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Scene/Systems.h"

namespace Engine {

    Scene::Scene(const std::string& name)
        : m_Name(name) {}

    Scene::~Scene() {}

    void Scene::onUpdate(Timestep ts) {
        
    }

    void Scene::onRenderRuntime(Timestep ts) {

    }

    void Scene::onRenderEditor(Timestep ts) {

    }

    void Scene::onEvent(Event& e) {

    }

    Entity Scene::getPrimaryCameraEntity() {
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view) {
            const auto& cameraComponent = view.get<CameraComponent>(entity);
            if (cameraComponent.primary) return Entity(entity, shared_from_this());
        }
    }

    Entity Scene::findEntityByTransformComponent(const TransformComponent& transform) {
        return Systems::FindEntityByTransformComponent(shared_from_this(), transform);
    }
    
    Entity Scene::createEntity(const std::string& name) {
        return Systems::CreateEntity(shared_from_this(), name);
    }

    Entity Scene::duplicateEntity(Entity entity) {
        return Systems::DuplicateEntity(entity, Systems::GetParentEntity(entity));
    }

    void Scene::destroyEntity(Entity entity, bool keepChildren) {
        Systems::DestroyEntity(entity, keepChildren);
    }    
}