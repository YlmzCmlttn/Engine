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
        std::weak_ptr<FrameBuffer> acquireFrameBuffer(const FrameBufferSpecification& spec);

        void releaseFrameBuffer(std::weak_ptr<FrameBuffer> fb);
        void resizeAllFrameBuffers(unsigned int width, unsigned int height);
    private:
        FrameBufferPool() = default;
        ~FrameBufferPool() = default;
        FrameBufferPool(const FrameBufferPool&) = delete;
        FrameBufferPool& operator=(const FrameBufferPool&) = delete;

    private:
        std::mutex m_poolMutex;
        std::vector<std::weak_ptr<FrameBuffer>> m_available;
    };

}