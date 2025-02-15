#include "Core/CommandQueue.h"


namespace Engine {

    void CommandQueue::submit(Command command) {
        m_CommandBuffer.emplace_back(std::move(command));  // Store lambda
    }

    void CommandQueue::execute() {
        for (auto& command : m_CommandBuffer) {
            command();  // Execute each stored command
        }
        m_CommandBuffer.clear();  // Clear after execution
    }

}
