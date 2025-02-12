#pragma once

#include "Core/Layer.h"
#include "Renderer/Mesh.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/Camera.h"    
#include "Renderer/TextureCube.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Uniforms.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"



class ExampleLayer : public Engine::Layer {
public:
    ExampleLayer();
    ~ExampleLayer() override = default;

    void onAttach() override;
    void onDetach() override;

    void onUpdate([[maybe_unused]] Engine::Timestep ts) override;
    void onEvent([[maybe_unused]] Engine::Event& event) override;
    void onImGuiRender() override;

private:
    std::unique_ptr<Engine::Shader> m_Shader;
	std::unique_ptr<Engine::Shader> m_PBRShader;
	std::unique_ptr<Engine::Shader> m_SimplePBRShader;
	std::unique_ptr<Engine::Shader> m_QuadShader;
	std::unique_ptr<Engine::Shader> m_HDRShader;
	std::unique_ptr<Engine::Mesh> m_Mesh;
	std::unique_ptr<Engine::Mesh> m_SphereMesh;
	std::unique_ptr<Engine::Texture2D> m_BRDFLUT;

	struct AlbedoInput
	{
		glm::vec3 Color = { 0.972f, 0.96f, 0.915f }; // Silver, from https://docs.unrealengine.com/en-us/Engine/Rendering/Materials/PhysicallyBased
		std::unique_ptr<Engine::Texture2D> TextureMap;
		bool SRGB = true;
		bool UseTexture = false;
	};
	AlbedoInput m_AlbedoInput;

	struct NormalInput
	{
		std::unique_ptr<Engine::Texture2D> TextureMap;
		bool UseTexture = false;
	};
	NormalInput m_NormalInput;

	struct MetalnessInput
	{
		float Value = 1.0f;
		std::unique_ptr<Engine::Texture2D> TextureMap;
		bool UseTexture = false;
	};
	MetalnessInput m_MetalnessInput;

	struct RoughnessInput
	{
		float Value = 0.5f;
		std::unique_ptr<Engine::Texture2D> TextureMap;
		bool UseTexture = false;
	};
	RoughnessInput m_RoughnessInput;

	std::unique_ptr<Engine::FrameBuffer> m_Framebuffer, m_FinalPresentBuffer;

	std::unique_ptr<Engine::VertexBuffer> m_VertexBuffer;
	std::unique_ptr<Engine::IndexBuffer> m_IndexBuffer;
	std::unique_ptr<Engine::TextureCube> m_EnvironmentCubeMap, m_EnvironmentIrradiance;

	Engine::Camera m_Camera;

	float m_ClearColor[4];

	struct Light
	{
		glm::vec3 Direction;
		glm::vec3 Radiance;
	};
	Light m_Light;
	float m_LightMultiplier = 0.3f;

	// PBR params
	float m_Exposure = 1.0f;

	bool m_RadiancePrefilter = false;

	float m_EnvMapRotation = 0.0f;

	enum class Scene : uint32_t
	{
		Spheres = 0, Model = 1
	};
	Scene m_Scene;

	// Editor resources
	std::unique_ptr<Engine::Texture2D> m_CheckerboardTex;
};
