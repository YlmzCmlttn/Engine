#include "Panels/ViewportPanel.h"
#include <imgui.h>
#include "Core/Application.h"

ViewportPanel::ViewportPanel(std::shared_ptr<Engine::FrameBuffer> framebuffer)
    : m_Framebuffer(framebuffer)
{
}

ViewportPanel::~ViewportPanel()
{
}

void ViewportPanel::onImGuiRender() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Viewport");
    m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();
    Engine::Application::Get().blockImGuiEvents(!m_ViewportFocused && !m_ViewportHovered);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_ViewportSize = {viewportPanelSize.x,viewportPanelSize.y};
    

    ImGui::Image((ImTextureID)m_Framebuffer->getColorAttachmentRendererID(), viewportPanelSize);
    ImGui::End();
    ImGui::PopStyleVar();
}
