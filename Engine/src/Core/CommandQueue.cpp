#include "Core/CommandQueue.h"


namespace Engine {

    void CommandQueue::submit(Command command) {
        m_CommandBuffer.push(std::move(command));  // Store lambda
    }

    void CommandQueue::execute() {
        while(!m_CommandBuffer.empty()) {
            m_CommandBuffer.front()();  // Execute each stored command
            m_CommandBuffer.pop();
        }
    }

}
