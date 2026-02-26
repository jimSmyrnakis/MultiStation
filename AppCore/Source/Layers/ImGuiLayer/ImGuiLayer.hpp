#pragma once
#include <Media.hpp>
#include <ImGui.hpp>
#include <stdint.h>
#include <stddef.h>

namespace MultiStation {
    ImGuiKey MS_to_ImGui_key(uint32_t ms_key)noexcept;
    uint32_t ImGui_to_MS_key(ImGuiKey imgui_key) noexcept;
    int MS_to_ImGui_mouse(int button) noexcept;
    uint32_t ImGui_to_MS_mouse(int button) noexcept;

    class ImguiLayer : public Layer {
    public:
        ImguiLayer(void) noexcept;
        ~ImguiLayer(void) noexcept;

        virtual void OnAttach(void) noexcept override;
        virtual void OnDetach(void) noexcept override;
        //virtual void OnEvent(Event& event)noexcept override;
        //virtual void OnUpdate(float deltaTime) noexcept override;
        virtual void OnImGuiRender(void) noexcept override;

        void Begin(void) noexcept;
        void End(void) noexcept;

    private:
        bool OnWindowResizeEvent(WindowResizeEvent& e) noexcept;
        bool OnWindowFocusEvent(WindowFocusEvent& e) noexcept;
        bool OnWindowCloseEvent(WindowCloseEvent& e) noexcept;
        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e) noexcept;
        bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e) noexcept;
        bool OnMouseMoveEvent(MouseMovedEvent& e) noexcept;
        bool OnMouseScrollEvent(MouseScrolledEvent& e) noexcept;
        bool OnKeyPressedEvent(KeyPressedEvent& e) noexcept;
        bool OnKeyReleasedEvent(KeyReleasedEvent& e) noexcept;
        bool OnKeyTypedEvent(KeyTypedEvent& e) noexcept;
    private:
        float m_Time = 0.0f;


    };

};
