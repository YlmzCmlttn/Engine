#include "Core/LayerStack.h"

namespace Engine {

    LayerStack::LayerStack() : m_LayerInsertIndex(0) {}

    LayerStack::~LayerStack() = default;

    void LayerStack::pushLayer(Ref<Layer> layer) {
        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
        m_LayerInsertIndex++;
    }

    void LayerStack::pushOverlay(Ref<Layer> overlay) {
        m_Layers.emplace_back(overlay);
    }

    void LayerStack::popLayer(Ref<Layer> layer) {
        auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
        if (it != m_Layers.begin() + m_LayerInsertIndex) {
            m_Layers.erase(it);
            m_LayerInsertIndex--;
        }
    }

    void LayerStack::popOverlay(Ref<Layer> overlay) {
        auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
        if (it != m_Layers.end()) {
            m_Layers.erase(it);
        }
    }

} // namespace Engine
