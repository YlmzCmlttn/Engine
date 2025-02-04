#pragma once
#include "Core.h"
#include "Layer.h"

namespace Engine{
    class LayerStack{
    public:
        LayerStack();
        ~LayerStack();

        void pushLayer(Ref<Layer> layer);
        void pushOverlay(Ref<Layer> overlay);
        void popLayer(Ref<Layer> layer);
        void popOverlay(Ref<Layer> overlay);

        std::vector<Ref<Layer>>::iterator begin() { return m_Layers.begin(); }
        std::vector<Ref<Layer>>::iterator end() { return m_Layers.end(); }
    private:
        std::vector<Ref<Layer>> m_Layers;
        unsigned int m_LayerInsertIndex = 0;
    };
}