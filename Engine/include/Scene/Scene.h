#pragma once

#include "Core/Timestep.h"
#include "Events/Event.h"
#include "entt/entity/registry.hpp"
namespace Engine {

    class Entity;

    class Scene {
    public:
        Scene(const std::string& name = "Untitled");
        ~Scene();        

        void onUpdate(Timestep ts);
        void onRenderRuntime(Timestep ts);
        void onRenderEditor(Timestep ts);
        void onEvent(Event& e);

        Entity createEntity(const std::string& name = std::string());
        void destroyEntity(Entity entity);
    private:
        entt::registry m_Registry;
        std::string m_Name;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
        friend class Entity;
    };
}