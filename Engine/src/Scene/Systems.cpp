#include "Scene/Entity.h"
#include "Scene/Systems.h"
#include "Scene/Components.h"

namespace Engine {

    Entity Systems::CreateEntity(Ref<Scene> scene, const std::string& name) {
        Entity entity =  Entity(scene->getRegistry().create(), scene);
        auto& tag = entity.addComponent<TagComponent>();
		tag.tag = name.empty() ? "Entity" : name;
        auto& relationship = entity.addComponent<RelationshipComponent>();
        auto& transform = entity.addComponent<TransformComponent>();
        entity.setParent(scene->getSceneEntity());
        return entity;
    }


    void Systems::onTransformComponentConstruct(entt::registry& registry, entt::entity entity) {
        registry.emplace<DirtyFlagComponent>(entity);
    }

    void Systems::onTransformComponentReplace(entt::registry& registry, entt::entity entity) {
        registry.emplace<DirtyFlagComponent>(entity);
    }

    void Systems::onDirtyFlagComponentConstruct(entt::registry& registry, entt::entity entity) {
        MarkChildrenDirty(registry, entity);
    }

    void Systems::onDirtyFlagComponentReplace(entt::registry& registry, entt::entity entity) {
        MarkChildrenDirty(registry, entity);
    }

    void Systems::MarkEntityAsDirty(entt::registry& registry, entt::entity entity) {
        registry.emplace<DirtyFlagComponent>(entity);
    }

    void Systems::MarkEntityAsDirty(Ref<Scene> scene, Entity entity) {
        MarkEntityAsDirty(scene->getRegistry(), entity.getHandle());
    }

    void Systems::MarkChildrenDirty(entt::registry& registry, entt::entity entity) {
        auto currChild = registry.get<RelationshipComponent>(entity).first.getHandle();
        auto numberOfChildren = registry.get<RelationshipComponent>(entity).children;
        for(int i = 0; i < numberOfChildren; i++) {
            MarkEntityAsDirty(registry, currChild);
            currChild = registry.get<RelationshipComponent>(currChild).next.getHandle();
        }
    }

    void Systems::MarkChildrenDirty(Ref<Scene> scene, Entity entity) {
        MarkChildrenDirty(scene->getRegistry(), entity.getHandle());
    }

    Entity Systems::CreateEmptyEntity(Ref<Scene> scene) {
        return Entity(scene->getRegistry().create(), scene);
    }

    void Systems::UpdateEntityLocalTransform(Entity entity) {
        entity.getComponent<TransformComponent>().updateLocalTransform();
    }

    void Systems::UpdateEntityGlobalTransform(Entity entity) {
        entity.getComponent<TransformComponent>().updateGlobalTransform();
    }

    void Systems::CalculateEntityGlobalTransformBasedOnParent(Entity entity) {
        auto parentTransform = TransformComponent();
        if(entity.getComponent<RelationshipComponent>().parent) {
            parentTransform = entity.getComponent<RelationshipComponent>().parent.getComponent<TransformComponent>();
        }
        auto& transform = entity.getComponent<TransformComponent>();
        UpdateEntityLocalTransform(entity);
        
        auto localTransform = transform.getLocalTransform();
        auto globalTransform = parentTransform.getGlobalTransform() * localTransform;
        transform.globalTransform = globalTransform;

        glm::vec3 Scale;
        glm::quat Orientation;
        glm::vec3 Translation;
        glm::vec3 Skew(1);
        glm::vec4 Perspective(1);
        glm::decompose(globalTransform, Scale, Orientation, Translation, Skew, Perspective);
        transform.position = Translation;
        transform.rotation = Orientation;
        transform.scale = Scale;
    }

    void Systems::PatchTransform(Ref<Scene> scene, Entity entity, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale){
        //use patch to set the transform
        scene->getRegistry().patch<TransformComponent>(entity.getHandle(), [&](auto& transform) {
            transform.localPosition = translation;
            transform.localRotation = glm::quat(rotation);
            transform.localScale = scale;
        });
    }

    void Systems::CalculateEntityLocalTransformBasedOnParent(Entity entity) {
        auto parentTransform = TransformComponent();
        if(entity.getComponent<RelationshipComponent>().parent) {
            parentTransform = entity.getComponent<RelationshipComponent>().parent.getComponent<TransformComponent>();
        }

        auto& transform = entity.getComponent<TransformComponent>();
        UpdateEntityGlobalTransform(entity);
        auto globalTransform = transform.getGlobalTransform();
        auto localTransform = glm::inverse(parentTransform.getGlobalTransform()) * globalTransform;
        transform.localTransform = localTransform;

        glm::vec3 Scale;
        glm::quat Orientation;
        glm::vec3 Translation;
        glm::vec3 Skew(1);
        glm::vec4 Perspective(1);
        glm::decompose(localTransform, Scale, Orientation, Translation, Skew, Perspective);
        transform.localPosition = Translation;
        transform.localRotation = Orientation;
        transform.localScale = Scale;
    }

    
    void Systems::UpdateTransforms(Ref<Scene> scene) {
        scene->getRegistry().sort<DirtyFlagComponent>([&](entt::entity lhs, entt::entity rhs) {
            const auto &clhs = scene->getRegistry().get<RelationshipComponent>(lhs);
            const auto &crhs = scene->getRegistry().get<RelationshipComponent>(rhs);
            return crhs.parent.getHandle() == lhs || clhs.next.getHandle() == rhs
                || (!(clhs.parent.getHandle() == rhs || crhs.next.getHandle() == lhs) && (clhs.parent.getHandle() < crhs.parent.getHandle() || (clhs.parent.getHandle() == crhs.parent.getHandle() && &clhs < &crhs)));
            });

        scene->getRegistry().view<DirtyFlagComponent>().each([&](auto entity) {
            CalculateEntityGlobalTransformBasedOnParent(Entity(entity, scene));
        });

        scene->getRegistry().clear<DirtyFlagComponent>();        
    }

    void Systems::DestroyEntity(Entity entity, bool keepChildren) {
        auto parent = GetParentEntity(entity);
        entity.removeParent();
        
        for(uint32_t i = 0; i < entity.getComponent<RelationshipComponent>().children; i++) {
            auto child = entity.getChild(i);
            if(keepChildren) {
                child.setParent(parent);
            }else{
                child.removeParent();
                child.getScene()->getRegistry().destroy(child);
            }
        }
        entity.getScene()->getRegistry().destroy(entity);
    }

    void Systems::DuplicateTransformComponent(Entity src, Entity dst, Entity parent) {
        
    }

    Entity Systems::GetParentEntity(Entity entity) {
        return entity.getComponent<RelationshipComponent>().parent;
    }
    void Systems::DuplicateEntity(Entity src, Entity dst) {
        DuplicateComponent<TransformComponent>(src, dst);        
        DuplicateComponent<TagComponent>(src, dst);
        DuplicateComponent<CameraComponent>(src, dst);
        for(int i = 0; i <GetChildCount(src); i++) {
            auto child = src.getChild(i);
            auto dstChild = CreateEntity(child.getScene());
            dstChild.setParent(dst);
            DuplicateEntity(child, dstChild);            
        }
    }

    Entity Systems::DuplicateEntity(Entity entity) {
        auto parent = GetParentEntity(entity);
        auto dst = CreateEntity(entity.getScene());
        dst.setParent(parent);
        DuplicateEntity(entity, dst);
        dst.getComponent<TagComponent>().tag = dst.getComponent<TagComponent>().tag+"_copy";
        return dst;
    }
    
    
    void Systems::SetParent(Entity child, Entity parent) {
        RemoveFromParent(child);

        if(parent) {
            AddChild(parent,child);
        }

        CalculateEntityLocalTransformBasedOnParent(child);
    }

    void Systems::ReorderEntity(Entity entity,Entity next) {
        auto &entityRel = entity.getComponent<RelationshipComponent>();
        auto &nextRel = next.getComponent<RelationshipComponent>();

        auto newParent = GetParentEntity(next);
        RemoveFromParent(entity);
        
        auto& newParentRel = newParent.getComponent<RelationshipComponent>();
        if(newParentRel.first == next) {
            entityRel.next = next;
            nextRel.prev = entity;            
            newParentRel.first = entity;
        }else{
            auto sibling = newParentRel.first;
            while(sibling){
                auto& siblingRel = sibling.getComponent<RelationshipComponent>();
                if(siblingRel.next == next) {
                    entityRel.prev = sibling;
                    entityRel.next = next;
                    siblingRel.next = entity;
                    nextRel.prev = entity;
                    break;
                }
                sibling = siblingRel.next;
            }
        }

        entityRel.parent = newParent;        
        newParentRel.children++;
    }

    void Systems::AddChild(Entity parent, Entity child) {
        auto &parentRel = parent.getComponent<RelationshipComponent>();
        auto &childRel = child.getComponent<RelationshipComponent>();

        childRel.parent = parent;

        childRel.prev = Entity();
        childRel.next = parentRel.first;
        
        // If there was already a first child, update its prev pointer.
        if (parentRel.first) {
            parentRel.first.getComponent<RelationshipComponent>().prev = child;
        }
        
        // Now make the child the first child.
        parentRel.first = child;
        ++parentRel.children;
    }


    void Systems::RemoveFromParent(Entity child) {
        auto &childRel = child.getComponent<RelationshipComponent>();
        auto &oldParent = childRel.parent;
        if(oldParent) {
            auto &parentRel = oldParent.getComponent<RelationshipComponent>();

            if(parentRel.first == child) {
                parentRel.first = childRel.next;
            }

            if(childRel.prev) {
                childRel.prev.getComponent<RelationshipComponent>().next = childRel.next;
            }

            if(childRel.next) {
                childRel.next.getComponent<RelationshipComponent>().prev = childRel.prev;
            }

            --parentRel.children;
        }

        childRel.parent = Entity();
        childRel.prev = Entity();
        childRel.next = Entity();

        CalculateEntityLocalTransformBasedOnParent(child);
    }

    uint32_t Systems::GetChildCount(Entity parent) {
        return parent.getComponent<RelationshipComponent>().children;
    }

    Entity Systems::GetChild(Entity parent, uint32_t index) {
        if(index >= GetChildCount(parent)) {
            std::cout << "Get Child: Index out of bounds" << std::endl;
            return Entity();
        }
        auto& relationship = parent.getComponent<RelationshipComponent>();
        auto curr = relationship.first;
        for(uint32_t i = 0; i < index; i++) {
            curr = curr.getComponent<RelationshipComponent>().next;
        }
        return curr;
    }
}