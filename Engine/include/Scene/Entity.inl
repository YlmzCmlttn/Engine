#pragma once
#include "Scene/Scene.h"  // Include the full definition for Scene

namespace Engine {

    template<typename T, typename... Args>
    T& Entity::addComponent(Args&&... args) {
        if(hasComponent<T>()) {
            std::cout << "Entity already has component, returning existing component" << std::endl;
            return getComponent<T>();
        }
        return m_Scene->getRegistry().emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    T& Entity::addOrReplaceComponent(Args&&... args) {
        return m_Scene->getRegistry().template emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
    }

    template<typename T>
    T& Entity::getComponent() {
        ENGINE_ASSERT(hasComponent<T>(), "Entity does not have component");
        return m_Scene->getRegistry().get<T>(m_EntityHandle);
    }

    template<typename T>
    bool Entity::hasComponent() const {
        return m_Scene->getRegistry().any_of<T>(m_EntityHandle);
    }

    template<typename T>
    void Entity::removeComponent() {
        m_Scene->getRegistry().remove<T>(m_EntityHandle);
    }

} // namespace Engine
