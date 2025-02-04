#pragma once

#include "RenderCommandQueue.h"

namespace Engine {

    class Renderer {
    public:
        static Renderer& get() {return *s_Instance;}

        void clear(float r, float g, float b, float a = 1.0f);
        void setClearColor(float r, float g, float b, float a);
        void waitAndRender();  // Executes all queued commands

        template <typename Func>
        static void submit(Func&& command) {
            get().m_CommandQueue.submit(std::forward<Func>(command));  // Submit to queue
        }

    private:
        static Renderer* s_Instance;
        RenderCommandQueue m_CommandQueue;
    };

}
