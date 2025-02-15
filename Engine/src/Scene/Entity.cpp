#include "Scene/Entity.h"
#include "Scene/Components.h"
#include "Scene/Systems.h"
namespace Engine {

    
    Entity::Entity(entt::entity handle, Ref<Scene> scene)
        : m_EntityHandle(handle), m_Scene(scene)
    {
    }

    void Entity::setParent(Entity parent){
        Systems::SetParent(*this, parent);
    }

    void Entity::removeParent(){
        Systems::RemoveFromParent(*this);
    }

    Entity Entity::getSceneEntity(){
        return Entity(m_Scene->m_SceneEntity, m_Scene);
    }

    Entity Entity::getChild(uint32_t index){
        return Systems::GetChild(*this, index);
    }

} // namespace Engine
