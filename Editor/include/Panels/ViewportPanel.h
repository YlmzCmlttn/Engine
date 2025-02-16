#pragma once

#include "Renderer/FrameBuffer.h"

class ViewportPanel{
public:
    ViewportPanel(std::shared_ptr<Engine::FrameBuffer> framebuffer);
    ~ViewportPanel();

    void onImGuiRender();

private:
    std::shared_ptr<Engine::FrameBuffer> m_Framebuffer;
};