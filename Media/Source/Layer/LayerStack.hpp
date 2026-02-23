#pragma once
#include "Layer.hpp"

namespace MultiStation {

    class LayerStack {
    public:
        LayerStack(void)noexcept;
        ~LayerStack(void)noexcept;

        void PushLayer(Layer* layer)noexcept;
        void PushOverlay(Layer* overlay)noexcept;
        void PopLayer(Layer* layer)noexcept;
        void PopOverlay(Layer* overlay)noexcept;

        std::vector<Layer*>::iterator begin(void)noexcept;
        std::vector<Layer*>::iterator end(void)noexcept;

    private:
        std::vector<Layer*>     m_Layers;
        uint32_t                m_LastIndex;
    };


};
