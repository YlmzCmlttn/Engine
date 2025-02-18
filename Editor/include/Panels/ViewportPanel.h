#pragma once

#include "Renderer/FrameBuffer.h"

class ViewportPanel{
public:
    ViewportPanel(std::shared_ptr<Engine::FrameBuffer> framebuffer);
    ~ViewportPanel();

    void onImGuiRender();

private:
    std::shared_ptr<Engine::FrameBuffer> m_Framebuffer;
    glm::vec2 m_ViewportSize = {0.0f,0.0f};
    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;;
};