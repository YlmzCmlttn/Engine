#pragma once
#include "Core/Assert.h"
#include "entt/entity/registry.hpp"
#include "glm/glm.hpp"
namespace Engine { class Scene; }

namespace Engine {
    class Entity {
    public:
        
        Entity() = default;
        Entity(entt::entity handle, Ref<Scene> scene);
        ~Entity()=default;

        template<typename T, typename... Args>
        T& addComponent(Args&&... args);

        template<typename T, typename... Args>
        T& addOrReplaceComponent(Args&&... args);

        template<typename T>
        T& getComponent();

        template<typename T>
        bool hasComponent() const;

        template<typename T>
        void removeComponent();


        inline operator bool() const { return m_EntityHandle != entt::null; }
        inline operator entt::entity() const { return m_EntityHandle; }
        inline operator uint32_t() const { return (uint32_t)m_EntityHandle; }

        inline bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
        inline bool operator!=(const Entity& other) const { return !(*this == other); }        

        Ref<Scene> getScene() const { return m_Scene; }
        void setParent(Entity parent);
        void removeParent();
        Entity getChild(uint32_t index);
        entt::entity getHandle() const { return m_EntityHandle; }

        void setTransform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);
        
    private:
        Entity getSceneEntity();
        entt::entity m_EntityHandle{entt::null};
        Ref<Scene> m_Scene;
    };
}
#include "Entity.inl"