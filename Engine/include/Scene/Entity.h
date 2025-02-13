#pragma once

#include "Scene/Scene.h"
#include "Core/Assert.h"
#include "entt/entity/registry.hpp"

namespace Engine {

    class Entity {
    public:
        Entity() = default;
        Entity(entt::entity handle, Ref<Scene> scene);
        ~Entity()=default;

        template<typename T, typename... Args>
        T& addComponent(Args&&... args){
            return m_Scene->getRegistry().emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& getComponent(){
            ENGINE_ASSERT(hasComponent<T>(), "Entity does not have component");
            return m_Scene->getRegistry().get<T>(m_EntityHandle);
        }

        template<typename T>
        bool hasComponent() const{
            return m_Scene->getRegistry().any_of<T>(m_EntityHandle);
        }

        template<typename T>
        void removeComponent(){
            m_Scene->getRegistry().remove<T>(m_EntityHandle);
        }


        inline operator bool() const { return m_EntityHandle != entt::null; }
        inline operator entt::entity() const { return m_EntityHandle; }
        inline operator uint32_t() const { return (uint32_t)m_EntityHandle; }

        inline bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
        inline bool operator!=(const Entity& other) const { return !(*this == other); }        

        Ref<Scene> getScene() const { return m_Scene; }
    private:
        entt::entity m_EntityHandle{entt::null};
        Ref<Scene> m_Scene;
    };
}