#pragma once

#include "Renderer/FrameBuffer.h"
#include "EditorScene/EditorScene.h"

class ViewportPanel{
public:
    ViewportPanel(std::shared_ptr<Engine::FrameBuffer> framebuffer);
    ~ViewportPanel();

    void onImGuiRender();
    inline void setActiveScene(std::shared_ptr<EditorScene> scene) { m_ActiveScene = scene;}
private:
    std::shared_ptr<Engine::FrameBuffer> m_Framebuffer;
    std::shared_ptr<EditorScene> m_ActiveScene;
    glm::vec2 m_ViewportSize = {0.0f,0.0f};
    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;;
};