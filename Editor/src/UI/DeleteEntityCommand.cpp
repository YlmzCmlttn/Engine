#include "UI/DeleteEntityCommand.h"
#include "Scene/Components.h"
#include "UI/UndoManager.h"
#include "Scene/Systems.h"
namespace Engine{
    DeleteEntityCommand::DeleteEntityCommand(Entity entity, bool keepChildren)
        : m_Entity(entity), m_KeepChildren(keepChildren)
    {
        m_Scene = entity.getScene();
        auto ID = m_Entity.getComponent<IdComponent>();
        auto tag = m_Entity.getComponent<TagComponent>();
        auto transform = m_Entity.getComponent<TransformComponent>();
        auto relationship = m_Entity.getComponent<RelationshipComponent>();
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
    }
    void DeleteEntityCommand::execute(){
        while(m_Entity.getComponent<RelationshipComponent>().first !=  Entity()) {
            auto child = m_Entity.getComponent<RelationshipComponent>().first;
            std::unique_ptr<Engine::Command> command = std::make_unique<Engine::DeleteEntityCommand>(child, true);
            Engine::UndoManager::get().executeCommand(command);
        }
        m_Scene->destroyEntity(m_Entity, true);
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
        // if(m_RestoreRelationshipComponent.prev) {
        //     auto prev = m_Scene->getEntityByUUID(m_RestoreRelationshipComponent.prev.value());
        //     m_Scene->reorderEntity(prev,newEntity);
        // }
        
        
    }
}