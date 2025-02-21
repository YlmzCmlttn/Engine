#include "UI/DuplicateEntityCommand.h"
namespace Engine{
    DuplicateEntityCommand::DuplicateEntityCommand(Entity entity)
        : EntityCommand(entity)
    {
    }
    void DuplicateEntityCommand::execute(){
        if(m_DeleteEntityCommand){
            m_DeleteEntityCommand->undo();
        }else{
            Entity duplicatedEntity = m_Scene->duplicateEntity(getEntity());
            m_DuplicatedID = duplicatedEntity.getComponent<IdComponent>().id;
            m_DeleteEntityCommand = std::make_unique<DeleteEntityCommand>(duplicatedEntity, true);
        }
    }
    void DuplicateEntityCommand::undo(){
        if(m_DeleteEntityCommand){
            m_DeleteEntityCommand->execute();
        }else{
            std::cout << "No delete entity command to undo Duplicate undo called before execute" << std::endl;
        }
    }
}