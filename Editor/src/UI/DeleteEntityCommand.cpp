#include "UI/DeleteEntityCommand.h"
#include "Scene/Components.h"
#include "UI/UndoManager.h"
#include "Scene/Systems.h"
namespace Engine{
    DeleteEntityCommand::DeleteEntityCommand(Entity entity, bool keepChildren)
        : m_Scene(entity.getScene()),m_ID(entity.getComponent<IdComponent>().id), m_KeepChildren(keepChildren)
    {
        auto ID = entity.getComponent<IdComponent>();
        auto tag = entity.getComponent<TagComponent>();
        auto transform = entity.getComponent<TransformComponent>();
        auto relationship = entity.getComponent<RelationshipComponent>();
        if(relationship.parent != Entity()) {   
            m_RestoreRelationshipComponent.parent  = std::make_optional<UUID>(relationship.parent.getComponent<IdComponent>().id);
        }
        if(relationship.next != Entity()) {
            m_RestoreRelationshipComponent.next = std::make_optional<UUID>(relationship.next.getComponent<IdComponent>().id);
        }
        if(relationship.prev != Entity()) {
            m_RestoreRelationshipComponent.prev = std::make_optional<UUID>(relationship.prev.getComponent<IdComponent>().id);
        }
        if(relationship.first != Entity()) {
            m_RestoreRelationshipComponent.first = std::make_optional<UUID>(relationship.first.getComponent<IdComponent>().id);
        }
        restoreComponents.push_back([ID, tag, transform](Entity entity) { 
            entity.addOrReplaceComponent<TransformComponent>(transform);
            entity.addOrReplaceComponent<TagComponent>(tag);
            entity.addOrReplaceComponent<IdComponent>(ID);
        });
        if(entity.hasComponent<CameraComponent>()) {
            auto camera = entity.getComponent<CameraComponent>();
            restoreComponents.push_back([camera](Entity entity) { 
                entity.addOrReplaceComponent<CameraComponent>(camera);
            });
        }
        if(entity.hasComponent<MeshComponent>()) {
            auto mesh = entity.getComponent<MeshComponent>();
            restoreComponents.push_back([mesh](Entity entity) { 
                entity.addOrReplaceComponent<MeshComponent>(mesh);
            });
        }
        if(entity.hasComponent<MeshRendererComponent>()) {
            auto meshRenderer = entity.getComponent<MeshRendererComponent>();
            restoreComponents.push_back([meshRenderer](Entity entity) { 
                entity.addOrReplaceComponent<MeshRendererComponent>(meshRenderer);
            });
        }   
    }
    void DeleteEntityCommand::execute(){
        auto entity = m_Scene->getEntityByUUID(m_ID);
        while(entity.getComponent<RelationshipComponent>().first !=  Entity()) {
            auto child = entity.getComponent<RelationshipComponent>().first;
            std::unique_ptr<Engine::DeleteEntityCommand> command = std::make_unique<Engine::DeleteEntityCommand>(child, true);
            //Engine::UndoManager::get().executeCommand(command);
            command->execute();
            m_DeleteQueue.push(std::move(command));
        }
        m_Scene->destroyEntity(entity, true);
    }
    void DeleteEntityCommand::undo(){
        Entity newEntity =  Entity(m_Scene->getRegistry().create(), m_Scene);
        auto& relationship = newEntity.addComponent<RelationshipComponent>();

        for(auto& restoreComponent : restoreComponents){
            restoreComponent(newEntity);
        }
        m_Scene->getEntityMap()[newEntity.getComponent<IdComponent>().id] = newEntity;
        if(m_RestoreRelationshipComponent.parent) {
            auto parent = m_Scene->getEntityByUUID(m_RestoreRelationshipComponent.parent.value());
            newEntity.setParent(parent);
        }
        if(m_RestoreRelationshipComponent.next) {
            auto next = m_Scene->getEntityByUUID(m_RestoreRelationshipComponent.next.value());
            m_Scene->reorderEntity(newEntity, next);
        }else{
            Systems::ReorderEntityToLast(newEntity);
        }

        while(!m_DeleteQueue.empty()){
            m_DeleteQueue.front()->undo();
            m_DeleteQueue.pop();
        }
    }
}