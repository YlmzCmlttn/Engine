#pragma once
#include "Core.h"
#include "Window.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/WindowEvent.h"
#include "Layer.h"
#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include "Core/CommandQueue.h"
namespace Engine
{
    class Application {
    public:
        Application(const std::string& name = "Application");

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;
        virtual ~Application();

        
        inline static Application& Get(){return *s_Instance;}

        virtual void run();

        virtual void onInit() {}
		virtual void onShutdown() {}
		virtual void onUpdate() {}

        virtual void onEvent(Event& e);

        void pushLayer(Ref<Layer> layer);
        void pushOverlay(Ref<Layer> overlay);
        void renderImGui();
        inline void blockImGuiEvents(bool block){m_ImGuiLayer->blockEvents(block);}

        void close();

        inline Window& getWindow(){return *m_Window;}
        template <typename Func>
        static void Submit(Func&& command) {
            Get().m_CommandQueue.submit(std::forward<Func>(command));
        }

    private:
        Ref<Window> m_Window;
        Ref<ImGuiLayer> m_ImGuiLayer;
        bool m_Running = true;
		bool m_Minimized = false;
        float m_LastFrameTime = 0.0f;
        LayerStack m_LayerStack;        

        static Application* s_Instance;
        CommandQueue m_CommandQueue;
        bool onWindowClose(WindowCloseEvent& e);
        bool onWindowResize(WindowResizeEvent& e);
    };

    Scope<Application> createApplication();
} // namespace Engine
