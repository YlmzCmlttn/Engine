#include "Scene/Scene.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"

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
            if (cameraComponent.primary) return Entity(entity, this);
        }
    }
    
    Entity Scene::createEntity(const std::string& name) {
        Entity entity =  Entity(m_Registry.create(), this);
        auto& tag = entity.addComponent<TagComponent>();
		tag.tag = name.empty() ? "Entity" : name;
        auto& transform = entity.addComponent<TransformComponent>();        
        return entity;
    }

    void Scene::destroyEntity(Entity entity) {
        m_Registry.destroy(entity);
    }    
}