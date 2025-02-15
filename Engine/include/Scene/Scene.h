#pragma once

#include "Core/Timestep.h"
#include "Events/Event.h"
#include "entt/entity/registry.hpp"

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
        

        Entity getPrimaryCameraEntity();
        entt::registry& getRegistry() { return m_Registry; }

        Entity getSceneEntity();
        void reorderEntity(Entity entity,Entity next);
    private:
        std::string m_Name;        
        entt::registry m_Registry;
        entt::entity m_SceneEntity;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
        friend class Entity;
    };
}