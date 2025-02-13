#pragma once
#include "entt/entity/registry.hpp"
#include "Scene/Components.h"
#include "Scene/Entity.h"
namespace Engine {
    class Systems {
    public:
        static Entity CreateEntity(Ref<Scene> scene, const std::string& name);
        static void DestroyEntity(Entity entity, bool keepChildren);
        static Entity DuplicateEntity(Entity entity, Entity parent=Entity());

        static void UpdateTransforms(Ref<Scene> scene);
        static Entity FindEntityByTransformComponent(Ref<Scene> scene, const TransformComponent& transform);

        template<typename Component>
        static void DuplicateComponent(Entity src, Entity dst) {
            if (src.hasComponent<Component>()) {
                const auto& srcComp = src.getComponent<Component>();
                // Assuming Component derives from ICloneableComponent:
                auto cloned = srcComp.clone();
                dst.addComponent<Component>(*static_cast<Component*>(cloned.get()));
            }
        }

        static Entity GetParentEntity(Entity entity);
    private:
        static Entity CreateEmptyEntity(Ref<Scene> scene);
        static void DuplicateTransformComponent(Entity src, Entity dst, Entity parent);
        
    };

    
}