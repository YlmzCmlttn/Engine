#include "Scene/Entity.h"
#include "Scene/Components.h"


namespace Engine {

    
    Entity::Entity(entt::entity handle, Ref<Scene> scene)
        : m_EntityHandle(handle), m_Scene(scene)
    {
    }

} // namespace Engine
