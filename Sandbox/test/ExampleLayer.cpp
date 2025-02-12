#include "ExampleLayer.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"

#include <imgui.h>

static void ImGuiShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
    }
}

ExampleLayer::ExampleLayer() : Layer("Example"),
 m_ClearColor{ 1.f, 0.1f, 0.1f, 1.0f }, m_Scene(Scene::Spheres),m_Camera(glm::perspectiveFov(glm::radians(45.0f), 1280.0f, 720.0f, 0.1f, 10000.0f))
{

}

void ExampleLayer::onAttach() {
    m_SimplePBRShader.reset(Engine::Shader::Create("assets/shaders/simplepbr.glsl"));
    m_QuadShader.reset(Engine::Shader::Create("assets/shaders/quad.glsl"));
    m_HDRShader.reset(Engine::Shader::Create("assets/shaders/hdr.glsl"));
    m_Mesh.reset(new Engine::Mesh("assets/meshes/cerberus.fbx"));
    m_SphereMesh.reset(new Engine::Mesh("assets/models/Sphere.fbx"));

    // Editor
    m_CheckerboardTex.reset(Engine::Texture2D::Create("assets/editor/Checkerboard.tga"));

    // Environment
    m_EnvironmentCubeMap.reset(Engine::TextureCube::Create("assets/textures/environments/Arches_E_PineTree_Radiance.tga"));
    m_EnvironmentIrradiance.reset(Engine::TextureCube::Create("assets/textures/environments/Arches_E_PineTree_Irradiance.tga"));
    m_BRDFLUT.reset(Engine::Texture2D::Create("assets/textures/BRDF_LUT.tga"));

    m_Framebuffer.reset(Engine::FrameBuffer::Create(Engine::FrameBufferFormat::RGBA16F, 1280, 720));
    m_FinalPresentBuffer.reset(Engine::FrameBuffer::Create(Engine::FrameBufferFormat::RGBA8, 1280, 720));

    // Create Quad
    float x = -1, y = -1;
    float width = 2, height = 2;
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec2 TexCoord;
    };

    QuadVertex* data = new QuadVertex[4];

    data[0].Position = glm::vec3(x, y, 0);
    data[0].TexCoord = glm::vec2(0, 0);

    data[1].Position = glm::vec3(x + width, y, 0);
    data[1].TexCoord = glm::vec2(1, 0);

    data[2].Position = glm::vec3(x + width, y + height, 0);
    data[2].TexCoord = glm::vec2(1, 1);

    data[3].Position = glm::vec3(x, y + height, 0);
    data[3].TexCoord = glm::vec2(0, 1);

    m_VertexBuffer.reset(Engine::VertexBuffer::Create());
    m_VertexBuffer->setData(data, 4 * sizeof(QuadVertex));

    uint32_t* indices = new uint32_t[6] { 0, 1, 2, 2, 3, 0, };
    m_IndexBuffer.reset(Engine::IndexBuffer::Create());
    m_IndexBuffer->setData(indices, 6 * sizeof(unsigned int));

    m_Light.Direction = { -0.5f, -0.5f, 1.0f };
    m_Light.Radiance = { 1.0f, 1.0f, 1.0f };
}

void ExampleLayer::onDetach() {
}

void ExampleLayer::onUpdate([[maybe_unused]] Engine::Timestep ts) {
    using namespace Engine;
    using namespace glm;

    m_Camera.Update();
    auto viewProjection = m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix();

    m_Framebuffer->bind();
    Renderer::Clear(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
/*
    Engine::UniformBufferDeclaration<sizeof(mat4), 1> quadShaderUB;
    quadShaderUB.push("u_InverseVP", inverse(viewProjection));
    m_QuadShader->uploadUniformBuffer(quadShaderUB);

    m_QuadShader->bind();
    m_EnvironmentIrradiance->bind(0);
    m_VertexBuffer->bind();
    m_IndexBuffer->bind();
    Renderer::DrawIndexed(m_IndexBuffer->getCount(), false);

    Engine::UniformBufferDeclaration<sizeof(mat4) * 2 + sizeof(vec3) * 4 + sizeof(float) * 8, 14> simplePbrShaderUB;
    simplePbrShaderUB.push("u_ViewProjectionMatrix", viewProjection);
    simplePbrShaderUB.push("u_ModelMatrix", mat4(1.0f));
    simplePbrShaderUB.push("u_AlbedoColor", m_AlbedoInput.Color);
    simplePbrShaderUB.push("u_Metalness", m_MetalnessInput.Value);
    simplePbrShaderUB.push("u_Roughness", m_RoughnessInput.Value);
    simplePbrShaderUB.push("lights.Direction", m_Light.Direction);
    simplePbrShaderUB.push("lights.Radiance", m_Light.Radiance * m_LightMultiplier);
    simplePbrShaderUB.push("u_CameraPosition", m_Camera.GetPosition());
    simplePbrShaderUB.push("u_RadiancePrefilter", m_RadiancePrefilter ? 1.0f : 0.0f);
    simplePbrShaderUB.push("u_AlbedoTexToggle", m_AlbedoInput.UseTexture ? 1.0f : 0.0f);
    simplePbrShaderUB.push("u_NormalTexToggle", m_NormalInput.UseTexture ? 1.0f : 0.0f);
    simplePbrShaderUB.push("u_MetalnessTexToggle", m_MetalnessInput.UseTexture ? 1.0f : 0.0f);
    simplePbrShaderUB.push("u_RoughnessTexToggle", m_RoughnessInput.UseTexture ? 1.0f : 0.0f);
    simplePbrShaderUB.push("u_EnvMapRotation", m_EnvMapRotation);
    m_SimplePBRShader->uploadUniformBuffer(simplePbrShaderUB);

    m_EnvironmentCubeMap->bind(10);
    m_EnvironmentIrradiance->bind(11);
    m_BRDFLUT->bind(15);
    
    m_SimplePBRShader->bind();
    if (m_AlbedoInput.TextureMap)
        m_AlbedoInput.TextureMap->bind(1);
    if (m_NormalInput.TextureMap)
        m_NormalInput.TextureMap->bind(2);
    if (m_MetalnessInput.TextureMap)
        m_MetalnessInput.TextureMap->bind(3);
    if (m_RoughnessInput.TextureMap)
        m_RoughnessInput.TextureMap->bind(4);

    if (m_Scene == Scene::Spheres)
    {
        // Metals
        float roughness = 0.0f;
        float x = -88.0f;
        for (int i = 0; i < 8; i++)
        {
            m_SimplePBRShader->setMat4("u_ModelMatrix", translate(mat4(1.0f), vec3(x, 0.0f, 0.0f)));
            m_SimplePBRShader->setFloat("u_Roughness", roughness);
            m_SimplePBRShader->setFloat("u_Metalness", 1.0f);
            m_SphereMesh->render();

            roughness += 0.15f;
            x += 22.0f;
        }

        // Dielectrics
        roughness = 0.0f;
        x = -88.0f;
        for (int i = 0; i < 8; i++)
        {
            m_SimplePBRShader->setMat4("u_ModelMatrix", translate(mat4(1.0f), vec3(x, 22.0f, 0.0f)));
            m_SimplePBRShader->setFloat("u_Roughness", roughness);
            m_SimplePBRShader->setFloat("u_Metalness", 0.0f);
            m_SphereMesh->render();

            roughness += 0.15f;
            x += 22.0f;
        }

    }
    else if (m_Scene == Scene::Model)
    {
        m_Mesh->render();
    }

    m_Framebuffer->unbind();

    m_FinalPresentBuffer->bind();
    m_HDRShader->bind();
    m_HDRShader->setFloat("u_Exposure", m_Exposure);
    m_Framebuffer->bindTexture();
    m_VertexBuffer->bind();
    m_IndexBuffer->bind();
    Renderer::DrawIndexed(m_IndexBuffer->getCount(), false);
    */
    m_Framebuffer->unbind();
}

void ExampleLayer::onEvent([[maybe_unused]] Engine::Event& event) {
    // Handle events if needed
}

void ExampleLayer::onImGuiRender() {
    static bool p_open = true;

		static bool opt_fullscreen_persistant = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		bool opt_fullscreen = opt_fullscreen_persistant;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (opt_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &p_open, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Dockspace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
		}

		// Editor Panel ------------------------------------------------------------------------------
		ImGui::Begin("Settings");
		if (ImGui::TreeNode("Shaders"))
		{
			auto& shaders = Engine::Shader::s_Shaders;
			for (auto& shader : shaders)
			{
				if (ImGui::TreeNode(shader->getName().c_str()))
				{
					std::string buttonName = "Reload##" + shader->getName();
					if (ImGui::Button(buttonName.c_str()))
						shader->reload();
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}

		ImGui::RadioButton("Spheres", (int*)&m_Scene, (int)Scene::Spheres);
		ImGui::SameLine();
		ImGui::RadioButton("Model", (int*)&m_Scene, (int)Scene::Model);

		ImGui::ColorEdit4("Clear Color", m_ClearColor);

		ImGui::SliderFloat3("Light Dir", glm::value_ptr(m_Light.Direction), -1, 1);
		ImGui::ColorEdit3("Light Radiance", glm::value_ptr(m_Light.Radiance));
		ImGui::SliderFloat("Light Multiplier", &m_LightMultiplier, 0.0f, 5.0f);
		ImGui::SliderFloat("Exposure", &m_Exposure, 0.0f, 10.0f);
		auto cameraForward = m_Camera.GetForwardDirection();
		ImGui::Text("Camera Forward: %.2f, %.2f, %.2f", cameraForward.x, cameraForward.y, cameraForward.z);

		ImGui::Separator();
		{
			ImGui::Text("Mesh");
			std::string fullpath = m_Mesh ? m_Mesh->getFilePath() : "None";
			size_t found = fullpath.find_last_of("/\\");
			std::string path = found != std::string::npos ? fullpath.substr(found + 1) : fullpath;
			ImGui::Text(path.c_str()); ImGui::SameLine();
			if (ImGui::Button("...##Mesh"))
			{
				std::string filename;
				if (filename != "")
					m_Mesh.reset(new Engine::Mesh(filename));
			}
		}
		ImGui::Separator();

		ImGui::Text("Shader Parameters");
		ImGui::Checkbox("Radiance Prefiltering", &m_RadiancePrefilter);
		ImGui::SliderFloat("Env Map Rotation", &m_EnvMapRotation, -360.0f, 360.0f);

		ImGui::Separator();

		// Textures ------------------------------------------------------------------------------
		{
			// Albedo
			if (ImGui::CollapsingHeader("Albedo", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
				ImGui::Image(m_AlbedoInput.TextureMap ? m_AlbedoInput.TextureMap->getRendererID() : m_CheckerboardTex->getRendererID(), ImVec2(64, 64));
				ImGui::PopStyleVar();
				if (ImGui::IsItemHovered())
				{
					if (m_AlbedoInput.TextureMap)
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted(m_AlbedoInput.TextureMap->getPath().c_str());
						ImGui::PopTextWrapPos();
						ImGui::Image(m_AlbedoInput.TextureMap->getRendererID(), ImVec2(384, 384));
						ImGui::EndTooltip();
					}
					if (ImGui::IsItemClicked())
					{
						std::string filename;
						if (filename != "")
							m_AlbedoInput.TextureMap.reset(Engine::Texture2D::Create(filename, m_AlbedoInput.SRGB));
					}
				}
				ImGui::SameLine();
				ImGui::BeginGroup();
				ImGui::Checkbox("Use##AlbedoMap", &m_AlbedoInput.UseTexture);
				if (ImGui::Checkbox("sRGB##AlbedoMap", &m_AlbedoInput.SRGB))
				{
					if (m_AlbedoInput.TextureMap)
						m_AlbedoInput.TextureMap.reset(Engine::Texture2D::Create(m_AlbedoInput.TextureMap->getPath(), m_AlbedoInput.SRGB));
				}
				ImGui::EndGroup();
				ImGui::SameLine();
				ImGui::ColorEdit3("Color##Albedo", glm::value_ptr(m_AlbedoInput.Color), ImGuiColorEditFlags_NoInputs);
			}
		}
		{
			// Normals
			if (ImGui::CollapsingHeader("Normals", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
				ImGui::Image(m_NormalInput.TextureMap ? m_NormalInput.TextureMap->getRendererID() : m_CheckerboardTex->getRendererID(), ImVec2(64, 64));
				ImGui::PopStyleVar();
				if (ImGui::IsItemHovered())
				{
					if (m_NormalInput.TextureMap)
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted(m_NormalInput.TextureMap->getPath().c_str());
						ImGui::PopTextWrapPos();
						ImGui::Image(m_NormalInput.TextureMap->getRendererID(), ImVec2(384, 384));
						ImGui::EndTooltip();
					}
					if (ImGui::IsItemClicked())
					{
						std::string filename;
						if (filename != "")
							m_NormalInput.TextureMap.reset(Engine::Texture2D::Create(filename));
					}
				}
				ImGui::SameLine();
				ImGui::Checkbox("Use##NormalMap", &m_NormalInput.UseTexture);
			}
		}
		{
			// Metalness
			if (ImGui::CollapsingHeader("Metalness", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
				ImGui::Image(m_MetalnessInput.TextureMap ? m_MetalnessInput.TextureMap->getRendererID() : m_CheckerboardTex->getRendererID(), ImVec2(64, 64));
				ImGui::PopStyleVar();
				if (ImGui::IsItemHovered())
				{
					if (m_MetalnessInput.TextureMap)
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted(m_MetalnessInput.TextureMap->getPath().c_str());
						ImGui::PopTextWrapPos();
						ImGui::Image(m_MetalnessInput.TextureMap->getRendererID(), ImVec2(384, 384));
						ImGui::EndTooltip();
					}
					if (ImGui::IsItemClicked())
					{
						std::string filename;
						if (filename != "")
							m_MetalnessInput.TextureMap.reset(Engine::Texture2D::Create(filename));
					}
				}
				ImGui::SameLine();
				ImGui::Checkbox("Use##MetalnessMap", &m_MetalnessInput.UseTexture);
				ImGui::SameLine();
				ImGui::SliderFloat("Value##MetalnessInput", &m_MetalnessInput.Value, 0.0f, 1.0f);
			}
		}
		{
			// Roughness
			if (ImGui::CollapsingHeader("Roughness", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
				ImGui::Image(m_RoughnessInput.TextureMap ? m_RoughnessInput.TextureMap->getRendererID() : m_CheckerboardTex->getRendererID(), ImVec2(64, 64));
				ImGui::PopStyleVar();
				if (ImGui::IsItemHovered())
				{
					if (m_RoughnessInput.TextureMap)
					{
						ImGui::BeginTooltip();
						ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
						ImGui::TextUnformatted(m_RoughnessInput.TextureMap->getPath().c_str());
						ImGui::PopTextWrapPos();
						ImGui::Image(m_RoughnessInput.TextureMap->getRendererID(), ImVec2(384, 384));
						ImGui::EndTooltip();
					}
					if (ImGui::IsItemClicked())
					{
						std::string filename;
						if (filename != "")
							m_RoughnessInput.TextureMap.reset(Engine::Texture2D::Create(filename));
					}
				}
				ImGui::SameLine();
				ImGui::Checkbox("Use##RoughnessMap", &m_RoughnessInput.UseTexture);
				ImGui::SameLine();
				ImGui::SliderFloat("Value##RoughnessInput", &m_RoughnessInput.Value, 0.0f, 1.0f);
			}
		}

		ImGui::Separator();

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");
		auto viewportSize = ImGui::GetContentRegionAvail();
		m_Framebuffer->resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		m_FinalPresentBuffer->resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		m_Camera.SetProjectionMatrix(glm::perspectiveFov(glm::radians(45.0f), viewportSize.x, viewportSize.y, 0.1f, 10000.0f));
		ImGui::Image(m_Framebuffer->getColorAttachmentRendererID(), viewportSize, { 0, 1 }, { 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Docking"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Flag: NoSplit", "", (opt_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 opt_flags ^= ImGuiDockNodeFlags_NoSplit;
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (opt_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  opt_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
				if (ImGui::MenuItem("Flag: NoResize", "", (opt_flags & ImGuiDockNodeFlags_NoResize) != 0))                opt_flags ^= ImGuiDockNodeFlags_NoResize;
				if (ImGui::MenuItem("Flag: PassthruDockspace", "", (opt_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))       opt_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (opt_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          opt_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
				ImGui::Separator();
				if (ImGui::MenuItem("Close DockSpace", NULL, false, p_open != NULL))
					p_open = false;
				ImGui::EndMenu();
			}
			ImGuiShowHelpMarker(
				"You can _always_ dock _any_ window into another by holding the SHIFT key while moving a window. Try it now!" "\n"
				"This demo app has nothing to do with it!" "\n\n"
				"This demo app only demonstrate the use of ImGui::DockSpace() which allows you to manually create a docking node _within_ another window. This is useful so you can decorate your main application window (e.g. with a menu bar)." "\n\n"
				"ImGui::DockSpace() comes with one hard constraint: it needs to be submitted _before_ any window which may be docked into it. Therefore, if you use a dock spot as the central point of your application, you'll probably want it to be part of the very first window you are submitting to imgui every frame." "\n\n"
				"(NB: because of this constraint, the implicit \"Debug\" window can not be docked into an explicit DockSpace() node, because that window is submitted as part of the NewFrame() call. An easy workaround is that you can create your own implicit \"Debug##2\" window after calling DockSpace() and leave it in the window stack for anyone to use.)"
			);

			ImGui::EndMenuBar();
		}

		ImGui::End();
}
