#include "../mspch.h"
#include "ImGuiLayer.hpp"
#include <GLAD.hpp>
#include <GLFW.hpp>
#include <ImGui.hpp>
#include "../Application/Application.hpp"
namespace MultiStation {


    ImGuiLayer::ImGuiLayer(void) noexcept : Layer("ImGui Layer") {

    }

    ImGuiLayer::~ImGuiLayer(void) noexcept {

    }






    void ImGuiLayer::OnAttach(void) noexcept {


        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); 
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

        GLFWwindow* window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }


    void ImGuiLayer::OnUIRender(float dt) noexcept
    {
        ImGui::Begin("MultiStation Debug");

        ImGui::Text("ImGui is working!");
        ImGui::Separator();

        ImGui::Text("Delta Time: %.4f ms", dt * 1000.0f);

        if (dt > 0.0f)
            ImGui::Text("FPS: %.1f", 1.0f / dt);

        static bool checkbox = false;
        ImGui::Checkbox("Test Checkbox", &checkbox);

        static float value = 0.5f;
        ImGui::SliderFloat("Test Value", &value, 0.0f, 1.0f);

        static int counter = 0;

        if (ImGui::Button("Click Me"))
            ++counter;

        ImGui::SameLine();
        ImGui::Text("Counter: %d", counter);

        ImGui::End();

        // Optional: official ImGui demo window
        static bool showDemo = true;

        if (showDemo)
            ImGui::ShowDemoWindow(&showDemo);
    }
    

    void ImGuiLayer::OnDetach(void) noexcept {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }


    void ImGuiLayer::Begin(void) noexcept {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::DockSpaceOverViewport();
        }
    }

    void ImGuiLayer::End(void) noexcept {
        ImGuiIO& io = ImGui::GetIO();
        Window* my_win = &Application::Get().GetWindow();

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
