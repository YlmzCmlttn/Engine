#include "Renderer/FrameBufferPool.h"

namespace Engine {


    std::weak_ptr<FrameBuffer> FrameBufferPool::acquireFrameBuffer(const FrameBufferSpecification& spec)
    {
        std::lock_guard<std::mutex> lock(m_poolMutex);

        auto it = std::find_if(m_available.begin(), m_available.end(),
            [spec](const std::weak_ptr<FrameBuffer>& fbWeak)
            {
                auto fb = fbWeak.lock(); // Convert weak_ptr to shared_ptr
                if (!fb) return false; // Skip expired weak pointers

                return (fb->getSpecification().width == spec.width &&
                        fb->getSpecification().height == spec.height);
            });

        if (it != m_available.end())
        {
            auto fbPtr = it->lock(); // Convert weak_ptr to shared_ptr before returning
            m_available.erase(it);
            return fbPtr;
        }

        auto newFB = FrameBuffer::Create(spec);
        return newFB;
    }

    void FrameBufferPool::releaseFrameBuffer(std::weak_ptr<FrameBuffer> fb)
    {
        std::lock_guard<std::mutex> lock(m_poolMutex);
        m_available.push_back(std::move(fb));
    }

    void FrameBufferPool::resizeAllFrameBuffers(unsigned int width, unsigned int height)
    {
        std::lock_guard<std::mutex> lock(m_poolMutex);
        for (auto& fbWeak : m_available)
        {
            auto fb = fbWeak.lock(); // Convert to shared_ptr
            if (fb)
            {
                fb->resize(width, height);
            }
        }
    }

}