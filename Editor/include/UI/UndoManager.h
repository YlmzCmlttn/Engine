#pragma once

#include "Command.h"

namespace Engine
{
    class UndoManager{
    public:
        static UndoManager& get();
        UndoManager(const UndoManager&) = delete;
        UndoManager& operator=(const UndoManager&) = delete;

        void setMaxUndoSteps(std::size_t maxUndoSteps);
        void executeCommand(std::unique_ptr<Command>& command);
        void undo();
        void redo();
        ~UndoManager() = default;
    private:
        explicit UndoManager(std::size_t maxUndoSteps = 100);
        std::size_t m_MaxUndoSteps;
        std::deque<std::unique_ptr<Command>> m_UndoDeque;
        std::deque<std::unique_ptr<Command>> m_RedoDeque;

        static std::unique_ptr<UndoManager> s_instance;
        static std::once_flag s_onceFlag;
    };
} // namespace Engine
