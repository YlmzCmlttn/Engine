#pragma once
#include "entt/entity/registry.hpp"
#include "Scene/Components.h"
#include "Scene/Entity.h"
namespace Engine {
    class Systems {
    public:
        static Entity CreateEntity(Ref<Scene> scene, const std::string& name = "");
        static void DestroyEntity(Entity entity, bool keepChildren);
        static Entity DuplicateEntity(Entity entity);

        static void UpdateTransforms(Ref<Scene> scene);

        template<typename Component>
        static void DuplicateComponent(Entity src, Entity dst) {
            if (src.hasComponent<Component>()) {
                const auto& srcComp = src.getComponent<Component>();
                // Assuming Component derives from ICloneableComponent:
                auto cloned = srcComp.clone();
                if(dst.hasComponent<Component>()) {
                    auto& dstComp = dst.getComponent<Component>();
                    dstComp = *static_cast<Component*>(cloned.get());
                }else{
                    dst.addComponent<Component>(*static_cast<Component*>(cloned.get()));
                }
            }
        }

        static void SetParent(Entity child, Entity parent);
        static void RemoveFromParent(Entity child);
        static Entity GetParentEntity(Entity entity);
        static Entity GetChild(Entity parent, uint32_t index);
        static uint32_t GetChildCount(Entity parent);
        static void ReorderEntity(Entity entity,Entity next);
    private:
        static void AddChild(Entity parent, Entity child);
        static Entity CreateEmptyEntity(Ref<Scene> scene);
        static void DuplicateTransformComponent(Entity src, Entity dst, Entity parent);
        static void DuplicateEntity(Entity src, Entity dst);
        
    };

    
}