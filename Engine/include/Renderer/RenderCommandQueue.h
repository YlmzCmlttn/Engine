#pragma once


namespace Engine {

    class RenderCommandQueue {
    public:
        using RenderCommand = std::function<void()>;  // Stores render commands as callable functions

        RenderCommandQueue() = default;
        ~RenderCommandQueue() = default;

        void submit(RenderCommand command);
        void execute();  // Executes all queued commands

    private:
        std::vector<RenderCommand> m_CommandBuffer;  // Stores the queued commands
    };
}
