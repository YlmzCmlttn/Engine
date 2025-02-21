#pragma once

#include "Scene/Entity.h"
#include "Core/Timestep.h"
#include "Events/Event.h"
#include "entt/entity/registry.hpp"
#include "glm/glm.hpp"
#include "Core/UUID.h"
namespace Engine {

	using EntityMap = std::unordered_map<UUID, Entity>;

    class Scene : public std::enable_shared_from_this<Scene> {
    public:
        Scene(const std::string& name = "Untitled");
        virtual ~Scene();        

        virtual void onAttach();
        virtual void onUpdate(Timestep ts);
        virtual void onRender(Timestep ts);
        virtual void onEvent(Event& e);

        Entity createEntity(const std::string& name = std::string());
        void destroyEntity(Entity entity, bool keepChildren = false);
        Entity duplicateEntity(Entity entity);
        

        Entity getPrimaryCameraEntity();
        entt::registry& getRegistry() { return m_Registry; }
        Entity getEntityByUUID(const UUID& uuid)const;
        Entity getSceneEntity();
        EntityMap& getEntityMap() { return m_EntityMap; }
        void reorderEntity(Entity entity,Entity next);
        Entity createEntityWithUUID(const std::string& name, const UUID& uuid);
    protected:
        virtual void onRender(Timestep ts, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
        virtual void onViewportResize(uint32_t width, uint32_t height);
    private:
        std::string m_Name;        
        entt::registry m_Registry;
        entt::entity m_SceneEntity;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
        EntityMap m_EntityMap;
        friend class Entity;
        friend class Systems;
    };
}