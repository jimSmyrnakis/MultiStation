#include "ImGuiLayer.hpp"
#include <ImGui.hpp>
#include "../../App/Application.hpp"
#define GLFW_STATIC
#include <GLFW/glfw3.h>
#include "../../App/Application.hpp"
namespace MultiStation {


    ImguiLayer::ImguiLayer(void) noexcept : Layer("ImGui Layer") {

    }
    
    ImguiLayer::~ImguiLayer(void) noexcept {

    }






    void ImguiLayer::OnAttach(void) noexcept {
        

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
#ifdef _WIN32 // only for windows
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
#endif
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        ImGuiStyle& style = ImGui::GetStyle();
        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        GLFWwindow* window = (GLFWwindow*)Application::GetInstance()->GetWindow()->GetNativeWindow();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    

    void ImguiLayer::OnImGuiRender(void) noexcept {
        static bool showDemo = true;
        ImGui::ShowDemoWindow(&showDemo);
    }
    
    void ImguiLayer::OnDetach(void) noexcept {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    
    
    void ImguiLayer::Begin(void) noexcept {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImguiLayer::End(void) noexcept {
        ImGuiIO& io = ImGui::GetIO();
        Window* my_win = Application::GetInstance()->GetWindow();

        io.DisplaySize = ImVec2(my_win->GetWidth(), my_win->GetHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* win = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(win);
        }
    }
    

    
}
