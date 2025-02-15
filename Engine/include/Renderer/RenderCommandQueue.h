#pragma once
#include "Core/CommandQueue.h"

namespace Engine {

    class RenderCommandQueue : public CommandQueue {
    public:
        using RenderCommand = CommandQueue::Command;  // Stores render commands as callable functions
    };
}
