#include "ImGuiLayer.hpp"
#define GLEW_STATIC
#include <GL/glew.h>
#include "../../App/Application.hpp"
namespace MultiStation {

   // void ImguiLayer::OnEvent(Event& event)noexcept {

       /* EventDispatcher dispatcher(event);
        
        // Window events
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(ImguiLayer::OnWindowResizeEvent));
        dispatcher.Dispatch<WindowFocusEvent>(BIND_EVENT_FN(ImguiLayer::OnWindowFocusEvent));
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(ImguiLayer::OnWindowCloseEvent));

        // Mouse Events
        dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(ImguiLayer::OnMouseButtonPressedEvent));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(ImguiLayer::OnMouseButtonReleasedEvent));
        dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(ImguiLayer::OnMouseMoveEvent));
        dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(ImguiLayer::OnMouseScrollEvent));

        // keyboard events
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImguiLayer::OnKeyPressedEvent));
        dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(ImguiLayer::OnKeyReleasedEvent));
        dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(ImguiLayer::OnKeyTypedEvent));

*/

  //  }
    
    bool ImguiLayer::OnWindowResizeEvent(WindowResizeEvent& e) noexcept {
        ImGuiIO& io = ImGui::GetIO();
        Window* win = Application::GetInstance()->GetWindow();
        io.DisplaySize = ImVec2(win->GetWidth(), win->GetHeight());
        glViewport(0, 0, win->GetWidth(), win->GetHeight());
        return false;
    }

    bool ImguiLayer::OnWindowFocusEvent(WindowFocusEvent& e) noexcept {
        ImGuiIO& io = ImGui::GetIO();
        io.AddFocusEvent(e.IsFocus());
        return false;
    }
    bool ImguiLayer::OnWindowCloseEvent(WindowCloseEvent& e) noexcept {
        ImGuiIO& io = ImGui::GetIO();
        
        return false;
    }





    bool ImguiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e) noexcept {
        ImGuiIO& io = ImGui::GetIO();
        int imgui_btn = MS_to_ImGui_mouse(e.GetMouseButton());
        io.AddMouseButtonEvent(imgui_btn, true);
        return false;
    }
    bool ImguiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e) noexcept {
        ImGuiIO& io = ImGui::GetIO();
        int imgui_btn = MS_to_ImGui_mouse(e.GetMouseButton());
        io.AddMouseButtonEvent(imgui_btn, false);
        return false;
    }
    bool ImguiLayer::OnMouseMoveEvent(MouseMovedEvent& e) noexcept {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMousePosEvent(e.GetX(), e.GetY());
        return false;
    }
    bool ImguiLayer::OnMouseScrollEvent(MouseScrolledEvent& e) noexcept {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMouseWheelEvent(e.GetXOffset(), e.GetYOffset());
        return false;
    }






    bool ImguiLayer::OnKeyPressedEvent(KeyPressedEvent& e) noexcept {
        ImGuiIO& io = ImGui::GetIO();
        ImGuiKey imgui_key = MS_to_ImGui_key( e.GetKeyCode() );
        if (imgui_key != ImGuiKey_None)
            io.AddKeyEvent(imgui_key, true);
        return false;
    }

    bool ImguiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e) noexcept {
        ImGuiIO& io = ImGui::GetIO();
        ImGuiKey imgui_key = MS_to_ImGui_key(e.GetKeyCode());
        if (imgui_key != ImGuiKey_None)
            io.AddKeyEvent(imgui_key, false);
        return false;
    }

    bool ImguiLayer::OnKeyTypedEvent(KeyTypedEvent& e) noexcept {
        ImGuiIO& io = ImGui::GetIO();

        io.AddInputCharacter(e.GetChar());

        return false;
    }

}
