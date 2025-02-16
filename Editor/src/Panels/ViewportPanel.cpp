#include "Panels/ViewportPanel.h"
#include <imgui.h>

ViewportPanel::ViewportPanel(std::shared_ptr<Engine::FrameBuffer> framebuffer)
    : m_Framebuffer(framebuffer)
{
}

ViewportPanel::~ViewportPanel()
{
}

void ViewportPanel::onImGuiRender() {
    ImGui::Begin("Viewport");
    ImGui::Image((ImTextureID)m_Framebuffer->getColorAttachmentRendererID(), ImVec2(1024,768));
    ImGui::End();
}
