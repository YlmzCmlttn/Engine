#pragma once

#include "Core/Timestep.h"
#include "Events/Event.h"
#include "entt/entity/registry.hpp"
#include "Scene/Components.h"
namespace Engine {

    class Entity;

    class Scene : public std::enable_shared_from_this<Scene> {
    public:
        Scene(const std::string& name = "Untitled");
        ~Scene();        

        void onUpdate(Timestep ts);
        void onRenderRuntime(Timestep ts);
        void onRenderEditor(Timestep ts);
        void onEvent(Event& e);

        Entity createEntity(const std::string& name = std::string());
        void destroyEntity(Entity entity, bool keepChildren = false);
        Entity duplicateEntity(Entity entity);

        Entity findEntityByTransformComponent(const TransformComponent& transform);

        Entity getPrimaryCameraEntity();
        entt::registry& getRegistry() { return m_Registry; }
    private:
        std::string m_Name;
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
        friend class Entity;
    };
}