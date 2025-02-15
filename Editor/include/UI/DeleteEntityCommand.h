#pragma once

#include "Command.h"

namespace Engine{
    class DeleteEntityCommand : public Command{
    public:
        DeleteEntityCommand(const std::string& name);
        void execute() override;
        void undo() override;
    private:
        std::string m_Name;
    };
}