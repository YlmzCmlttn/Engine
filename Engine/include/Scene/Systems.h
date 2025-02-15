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
        static void onTransformComponentConstruct(entt::registry& registry, entt::entity entity);
        static void onTransformComponentReplace(entt::registry& registry, entt::entity entity);

        static void CalculateEntityGlobalTransformBasedOnParent(Entity entity);
        static void CalculateEntityLocalTransformBasedOnParent(Entity entity);
        static void PatchTransform(Ref<Scene> scene, Entity entity, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);
        static void onDirtyFlagComponentConstruct(entt::registry& registry, entt::entity entity);
        static void onDirtyFlagComponentReplace(entt::registry& registry, entt::entity entity);

        static void MarkEntityAsDirty(Ref<Scene> scene, Entity entity);
        static void MarkEntityAsDirty(entt::registry& registry, entt::entity entity);
        static void MarkChildrenDirty(Ref<Scene> scene, Entity entity);
        static void MarkChildrenDirty(entt::registry& registry, entt::entity entity);
    private:
        static void AddChild(Entity parent, Entity child);
        static Entity CreateEmptyEntity(Ref<Scene> scene);
        static void DuplicateTransformComponent(Entity src, Entity dst, Entity parent);
        static void DuplicateEntity(Entity src, Entity dst);
        static void UpdateEntityGlobalTransform(Entity entity);
        static void UpdateEntityLocalTransform(Entity entity);
        
    };

    
}