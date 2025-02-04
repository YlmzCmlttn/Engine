#pragma once

#include "../Core/Layer.h"
#include "../Events/ApplicationEvent.h"
#include "../Events/MouseEvent.h"
#include "../Events/KeyEvent.h"

namespace Engine
{
    class ImGuiLayer : public Layer
    {
    private:
        float m_Time = 0.0f;
        bool m_BlockEvents = true;
        /* data */
    public:
        ImGuiLayer();
        ImGuiLayer(const std::string& name);
		~ImGuiLayer();

		virtual void onAttach() override;
		virtual void onDetach() override;
        virtual void onEvent(Event& e) override;
		virtual void onImGuiRender() override;

        void blockEvents(bool block) { m_BlockEvents = block; }
        void setDarkThemeColors();
		void begin();
		void end();
    };
    
}
