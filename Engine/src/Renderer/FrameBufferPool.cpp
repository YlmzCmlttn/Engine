#include "Renderer/FrameBufferPool.h"

namespace Engine {

    Ref<FrameBuffer> FrameBufferPool::acquireFrameBuffer(FrameBufferFormat format, unsigned int width, unsigned int height)
    {
        std::lock_guard<std::mutex> lock(m_poolMutex);

        auto it = std::find_if(m_available.begin(), m_available.end(),
            [width, height](const Ref<FrameBuffer>& fb)
            {
                return (fb->getWidth() == width && fb->getHeight() == height);
            });

        if (it != m_available.end())
        {
            auto fbPtr = *it;
            m_available.erase(it);
            return fbPtr;
        }

        auto newFB = FrameBuffer::Create(format, width, height);
        return newFB;
    }

    void FrameBufferPool::releaseFrameBuffer(Ref<FrameBuffer> fb)
    {
        std::lock_guard<std::mutex> lock(m_poolMutex);
        m_available.push_back(std::move(fb));
    }

    void FrameBufferPool::resizeAllFrameBuffers(unsigned int width, unsigned int height)
    {
        std::lock_guard<std::mutex> lock(m_poolMutex);
        for (auto& fb : m_available)
        {
            fb->resize(width, height);
        }
    }

}