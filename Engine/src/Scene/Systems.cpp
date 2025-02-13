#include "Scene/Systems.h"
#include "Scene/Components.h"

namespace Engine {

    Entity Systems::CreateEntity(Ref<Scene> scene, const std::string& name) {
        Entity entity =  Entity(scene->getRegistry().create(), scene);
        auto& tag = entity.addComponent<TagComponent>();
		tag.tag = name.empty() ? "Entity" : name;
        auto& transform = entity.addComponent<TransformComponent>();
        return entity;
    }

    Entity Systems::CreateEmptyEntity(Ref<Scene> scene) {
        return Entity(scene->getRegistry().create(), scene);
    }
    
    void Systems::UpdateTransforms(Ref<Scene> scene) {
        auto view = scene->getRegistry().view<TransformComponent>();
        for(auto entity : view) {
            auto &transform = view.get<TransformComponent>(entity);            
            if(transform.parent == nullptr){
                transform.updateTransform();
            }
        }
    }

    Entity Systems::FindEntityByTransformComponent(Ref<Scene> scene, const TransformComponent& transform) {
        auto view = scene->getRegistry().view<TransformComponent>();
        for (auto entity : view) {
            if (&view.get<TransformComponent>(entity) == &transform) return Entity(entity, scene);
        }
        return Entity();
    }

    void Systems::DestroyEntity(Entity entity, bool keepChildren) {
        Ref<Scene> scene = entity.getScene();
        if (entity.hasComponent<TransformComponent>()) {
            auto& transform = entity.getComponent<TransformComponent>();

            // Make a copy of children since the vector might be modified as we delete or reparent.
            auto childrenCopy = transform.children;

            if (!keepChildren) {
                transform.removeParent();
                // Recursively delete all children.
                for (auto childTransform : childrenCopy) {
                    Entity childEntity = FindEntityByTransformComponent(scene, *childTransform);
                    if (childEntity) {
                        DestroyEntity(childEntity, false);
                    }
                }
            } else {
                TransformComponent* newParent = transform.parent; // Could be nullptr.
                transform.removeParent();
                for (auto childTransform : childrenCopy) {
                    Entity childEntity = FindEntityByTransformComponent(scene, *childTransform);
                    if (childEntity) {
                        auto& childTC = childEntity.getComponent<TransformComponent>();
                        if (newParent) {
                            // Reparent child to the deleted entity's parent.
                            childTC.setParent(*newParent, true);
                        } else {
                            // No parent exists, so simply remove the parent.
                            childTC.removeParent();
                        }
                    }
                }
            }
        }

        // Optionally: remove the entity from any other systems/components before deletion.
        // Finally, remove the entity from the registry.
        scene->getRegistry().destroy((entt::entity)entity);
    }

    void Systems::DuplicateTransformComponent(Entity src, Entity dst, Entity parent) {
        DuplicateComponent<TransformComponent>(src, dst);
        if(parent){
            auto& dstTransform = dst.getComponent<TransformComponent>();
            auto& parentTransform = parent.getComponent<TransformComponent>();
            dstTransform.setParent(parentTransform, false);
        }
        for(auto child : src.getComponent<TransformComponent>().children){
            Entity childEntity = FindEntityByTransformComponent(src.getScene(), *child);
            if(childEntity){
                DuplicateTransformComponent(childEntity, dst, dst);
            }
        }
    }

    Entity Systems::GetParentEntity(Entity entity) {
        auto& transform = entity.getComponent<TransformComponent>();
        if(transform.parent){
            return FindEntityByTransformComponent(entity.getScene(), *transform.parent);
        }
        return Entity();
    }


    Entity Systems::DuplicateEntity(Entity entity, Entity parent) {
        Entity newEntity = CreateEmptyEntity(entity.getScene());        
        DuplicateComponent<TagComponent>(entity, newEntity);
        DuplicateComponent<CameraComponent>(entity, newEntity);
        DuplicateComponent<TransformComponent>(entity, newEntity);
        if(parent){
            auto& dstTransform = newEntity.getComponent<TransformComponent>();
            auto& parentTransform = parent.getComponent<TransformComponent>();
            dstTransform.setParent(parentTransform, false);
        }
        for(auto child : entity.getComponent<TransformComponent>().children){
            Entity childEntity = FindEntityByTransformComponent(entity.getScene(), *child);
            if(childEntity){
                DuplicateEntity(childEntity, newEntity);
            }
        }
        return newEntity;
    }        
}