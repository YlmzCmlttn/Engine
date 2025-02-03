#pragma once
#include "Core.h"
#include "Window.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/WindowEvent.h"
#include "Layer.h"
#include "LayerStack.h"

namespace Engine
{
    class Application {
    public:
        Application(const std::string& name = "Application");

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        
        inline static Application& Get(){return *s_Instance;}

        virtual void run();
        virtual void onEvent(Event& e);

        void pushLayer(Ref<Layer> layer);
        void pushOverlay(Ref<Layer> overlay);


        virtual ~Application();
    private:
        Scope<Window> m_Window;
        bool m_Running = true;
		bool m_Minimized = false;
        float m_LastFrameTime = 0.0f;
        LayerStack m_LayerStack;


        static Application* s_Instance;

        bool onWindowClose(WindowCloseEvent& e);
        bool onWindowResize(WindowResizeEvent& e);
    };

    Scope<Application> createApplication();
} // namespace Engine
