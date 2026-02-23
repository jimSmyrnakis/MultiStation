#include "ImGuiLayer.hpp"
#include <ImGui.hpp>
#include "../../App/Application.hpp"
namespace MultiStation {

    ImguiLayer::ImguiLayer(void) noexcept : Layer("ImGui Layer") {

    }
    
    ImguiLayer::~ImguiLayer(void) noexcept {

    }






    void ImguiLayer::OnAttach(void) noexcept {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImguiLayer::OnUpdate(float deltaTime) noexcept {
        ImGuiIO& io = ImGui::GetIO();
        Window* my_win = Application::GetInstance()->GetWindow();

        io.DeltaTime = deltaTime;
        io.DisplaySize = ImVec2(my_win->GetWidth(), my_win->GetHeight());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        
        static bool showDemo = true;
        ImGui::ShowDemoWindow(&showDemo);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImguiLayer::OnImGuiRender(void) noexcept {

    }
    
    void ImguiLayer::OnDetach(void) noexcept {

    }
    
    

    void ImguiLayer::OnEvent(Event& event)noexcept {

    }

    
}
