#pragma once
#include <Media.hpp>

namespace MultiStation {

    class ImguiLayer : public Layer {
    public:
        ImguiLayer(void) noexcept ;
        ~ImguiLayer(void) noexcept ;

        void OnAttach(void) noexcept override;
        void OnDetach(void) noexcept override;
        void OnImGuiRender(void) noexcept override;
        void OnEvent(Event& event)noexcept override;
        void OnUpdate(float deltaTime) noexcept override;
        /*void begin(void) noexcept;
        void end(void) noexcept ;
        */
    private:
        float m_Time = 0.0f;


    };

}
