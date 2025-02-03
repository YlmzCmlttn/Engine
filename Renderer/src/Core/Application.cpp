#include "Core/Application.h"
#include "Core/Assert.h"
#include <GLFW/glfw3.h>

namespace Engine {
    #define BIND_EVENT_FN(x) std::bind(&Application::x,this,std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name) {
        ENGINE_CORE_ASSERT(!s_Instance,"Application already exists!");
        s_Instance = this;
        m_Window = Window::Create();

        m_Window->setEventCallback(BIND_EVENT_FN(onEvent));

        //m_ImGuiLayer = new ImGuiLayer();
        //pushOverlay(m_ImGuiLayer);
    }



    Application::~Application() {
        std::cout << "Application destroyed" << std::endl;
        m_Running = false;
    }

    void Application::pushLayer(Ref<Layer> layer){
        m_LayerStack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::pushOverlay(Ref<Layer> overlay){
        m_LayerStack.pushOverlay(overlay);
        overlay->onAttach();
    }

    void Application::onEvent(Event& e){
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));
        dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(onWindowResize));

        for(auto it = m_LayerStack.end(); it != m_LayerStack.begin();){
            (*--it)->onEvent(e);
            if(e.isHandled())
                break;
        }
    }

    void Application::run() {
        while(m_Running){
            float time = m_Window->getTime();
            Timestep ts = time - m_LastFrameTime;
            m_LastFrameTime = time;

            for(Ref<Layer> layer : m_LayerStack){
                layer->onUpdate(ts);
            }
            //m_ImGuiLayer->begin();
            //for(Layer* layer : m_LayerStack){
            //    layer->onImGuiRender();
            //}
            //m_ImGuiLayer->end();

            m_Window->onUpdate();
        }
    }

    bool Application::onWindowClose([[maybe_unused]]WindowCloseEvent& e){
        m_Running=false;
        return true;
    }
    bool Application::onWindowResize(WindowResizeEvent& e)
	{
		if (e.getWidth() == 0 || e.getHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		//Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}
