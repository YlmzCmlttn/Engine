#pragma once

#include "Renderer/FrameBuffer.h"
namespace Engine {

    class FrameBufferPool
    {
    public:
        static FrameBufferPool& getInstance()
        {
            static FrameBufferPool instance;
            return instance;
        }
        Ref<FrameBuffer> acquireFrameBuffer(FrameBufferFormat format, unsigned int width, unsigned int height);

        void releaseFrameBuffer(Ref<FrameBuffer> fb);
        void resizeAllFrameBuffers(unsigned int width, unsigned int height);
    private:
        FrameBufferPool() = default;
        ~FrameBufferPool() = default;
        FrameBufferPool(const FrameBufferPool&) = delete;
        FrameBufferPool& operator=(const FrameBufferPool&) = delete;

    private:
        std::mutex m_poolMutex;
        std::vector<Ref<FrameBuffer>> m_available;
    };

}