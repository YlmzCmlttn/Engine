#include "UI/DeleteEntityCommand.h"

namespace Engine{
    DeleteEntityCommand::DeleteEntityCommand(const std::string& name)
        : m_Name(name)
    {
    }
    void DeleteEntityCommand::execute(){
        std::cout << m_Name + "DeleteEntityCommand::execute()" << std::endl;
    }
    void DeleteEntityCommand::undo(){
        std::cout << m_Name + "DeleteEntityCommand::undo()" << std::endl;
    }
}