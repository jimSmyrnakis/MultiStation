#pragma once
#include "Layer.hpp"

/**
 * @author Dimitris Smyrnakis
 * @file LayerStack 
 * @brief Used for events and final render to the screen / window in a stack form
 */
namespace MultiStation {

    class LayerStack {
    public:
        LayerStack(void)noexcept;
        ~LayerStack(void)noexcept;

        void PushLayer(Layer* layer)noexcept;
        void PushOverlay(Layer* overlay)noexcept;
        void PopLayer(Layer* layer)noexcept;
        void PopOverlay(Layer* overlay)noexcept;

        void OnEvent(Event& e);

        void OnUIRender(float dt);

        std::vector<Layer*>::iterator begin(void)noexcept;
        std::vector<Layer*>::iterator end(void)noexcept;

    private:
        std::vector<Layer*>     m_Layers;
        uint32_t                m_LastIndex;
    };


};
