#pragma once

#include "UI/EntityCommands.h"
#include "UI/DeleteEntityCommand.h"
namespace Engine{
    class DuplicateEntityCommand : public EntityCommand{
    public:
        DuplicateEntityCommand(Entity entity);
        void execute() override;
        void undo() override;
    private:
        UUID m_DuplicatedID;
        std::unique_ptr<DeleteEntityCommand> m_DeleteEntityCommand;
    };
}