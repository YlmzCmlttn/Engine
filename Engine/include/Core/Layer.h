#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Timestep.h"

namespace Engine{
    class Layer{
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void onAttach(){}
        virtual void onDetach(){}
        virtual void onUpdate([[maybe_unused]] Timestep ts){}
        virtual void onEvent([[maybe_unused]] Event& event){}
        virtual void onImGuiRender(){}

        inline const std::string& getName() const {return m_DebugName;}
    private:
        std::string m_DebugName;
    };
}