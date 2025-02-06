#pragma once

namespace Engine
{
    using RendererID = unsigned int;

	enum class RendererAPIType
	{
		None,
		OpenGL
	};

    struct RenderAPICapabilities
	{
		std::string Vendor;
		std::string Renderer;
		std::string Version;

		int MaxSamples;
		float MaxAnisotropy;
	};

    class RendererAPI
    {
    public:
        static void Init();
        static void Shutdown();
        static void Clear(float r, float g, float b, float a);
        static void SetClearColor(float r, float g, float b, float a);

        static void DrawIndexed(unsigned int indexCount, bool depthTesting = true);

        static RenderAPICapabilities& GetCapabilities() { 
            static RenderAPICapabilities capabilities;
            return capabilities; 
        }

        static RendererAPIType Current() { return s_CurrentRendererAPI; }

    private:
        static RendererAPIType s_CurrentRendererAPI;
    };
} // namespace Engine
