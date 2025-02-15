#pragma once


namespace Engine {

    class CommandQueue {
    public:
        using Command = std::function<void()>;  // Stores render commands as callable functions

        CommandQueue() = default;
        virtual ~CommandQueue() = default;

        virtual void submit(Command command);
        virtual void execute();

    private:
        std::vector<Command> m_CommandBuffer;  // Stores the queued commands
    };
}
