#include "UI/UndoManager.h"

namespace Engine{

    std::unique_ptr<UndoManager> UndoManager::s_instance;
    std::once_flag UndoManager::s_onceFlag;

    UndoManager::UndoManager(std::size_t maxUndoSteps)
        : m_MaxUndoSteps(maxUndoSteps)
    {

    }
    void UndoManager::setMaxUndoSteps(std::size_t maxUndoSteps){
        m_MaxUndoSteps = maxUndoSteps;
        while(m_UndoDeque.size() > m_MaxUndoSteps){
            m_UndoDeque.pop_front();
        }
    }

    UndoManager& UndoManager::get(){
        std::call_once(s_onceFlag, [](){
            s_instance.reset(new UndoManager());
        });
        return *s_instance;
    }
    void UndoManager::executeCommand(std::unique_ptr<Command>& command){
        command->execute();
        if(m_UndoDeque.size() >= m_MaxUndoSteps){
            m_UndoDeque.pop_front();
        }
        m_UndoDeque.push_back(std::move(command));
        m_RedoDeque.clear();
    }
    void UndoManager::undo(){
        if(!m_UndoDeque.empty()){
            auto command = std::move(m_UndoDeque.back());
            m_UndoDeque.pop_back();
            command->undo();
            m_RedoDeque.push_back(std::move(command));
        }
    }
    void UndoManager::redo(){
        if(!m_RedoDeque.empty()){
            auto command = std::move(m_RedoDeque.back());
            m_RedoDeque.pop_back();
            command->execute();
            m_UndoDeque.push_back(std::move(command));
        }
    }
    
}