#pragma once

#include "RenderCommandQueue.h"
#include "RendererAPI.h"

namespace Engine {

    class Renderer {
    public:
        static Renderer& get() {return *s_Instance;}
        static void Init();
        static void Clear();
        static void Clear(float r, float g, float b, float a = 1.0f);
        static void SetClearColor(float r, float g, float b, float a);
        static void DrawIndexed(unsigned int indexCount,bool depthTest = true);
        void waitAndRender();  // Executes all queued commands

        template <typename Func>
        static void Submit(Func&& command) {
            get().m_CommandQueue.submit(std::forward<Func>(command));  // Submit to queue
        }

    private:
        static Renderer* s_Instance;
        RenderCommandQueue m_CommandQueue;
    };
}
