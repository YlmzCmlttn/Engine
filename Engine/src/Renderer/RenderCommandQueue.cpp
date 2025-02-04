#include "Renderer/RenderCommandQueue.h"

namespace Engine {

    void RenderCommandQueue::submit(RenderCommand command) {
        m_CommandBuffer.emplace_back(std::move(command));  // Store lambda
    }

    void RenderCommandQueue::execute() {
        for (auto& command : m_CommandBuffer) {
            command();  // Execute each stored command
        }
        m_CommandBuffer.clear();  // Clear after execution
    }

}
