#include "LayerStack.hpp"

namespace MultiStation {


    LayerStack::LayerStack(void) noexcept {
        m_LastIndex = 0;
    }

    LayerStack::~LayerStack(void) noexcept {
        for (Layer* layer : m_Layers)
            delete layer;
    }

    void LayerStack::PushLayer(Layer* layer) noexcept {
        m_Layers.emplace(m_Layers.begin() + m_LastIndex, layer);
        m_LastIndex++;
        layer->OnAttach();
    }

    void LayerStack::PushOverlay(Layer* overlay) noexcept {
        m_Layers.emplace_back(overlay);
        overlay->OnAttach();
    }

    void LayerStack::PopLayer(Layer* layer) noexcept {
        std::vector<Layer*>::iterator it = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (it == m_Layers.end())
            return;

        layer->OnDetach();
        m_Layers.erase(it);
        m_LastIndex--;

    }

    void LayerStack::PopOverlay(Layer* overlay) noexcept {
        std::vector<Layer*>::iterator it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
        if (it == m_Layers.end())
            return;

        (*it)->OnDetach();
        m_Layers.erase(it);
    }

    std::vector<Layer*>::iterator LayerStack::begin(void) noexcept { return m_Layers.begin(); }

    std::vector<Layer*>::iterator LayerStack::end(void) noexcept { return m_Layers.end(); }

}
