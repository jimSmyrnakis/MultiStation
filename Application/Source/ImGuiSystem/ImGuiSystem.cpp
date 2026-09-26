#include "../mspch.h"
#include "ImGuiSystem.hpp"
#include <GLAD.hpp>
#include <GLFW.hpp>
#include <ImGui.hpp>
#include "../Application/Application.hpp"
namespace MultiStation {


    ImGuiSystem::ImGuiSystem(void) noexcept : IMSSystem("ImGui Layer") {

    }

    ImGuiSystem::~ImGuiSystem(void) noexcept {

    }






    void ImGuiSystem::OnAttach(void) noexcept {


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
        // Setup Dear ImGui s
        // tyle
        io.Fonts->AddFontFromFileTTF("./Assets/fonts/Consolas/CONSOLAB.ttf", 18);
        io.Fonts->AddFontFromFileTTF("./Assets/fonts/Consolas/Consolas.ttf", 18);
        io.Fonts->AddFontFromFileTTF("./Assets/fonts/Consolas/consolai.ttf", 18);
        io.Fonts->AddFontFromFileTTF("./Assets/fonts/Consolas/consolaz.ttf", 18);
        io.FontDefault = io.Fonts->AddFontFromFileTTF("./Assets/fonts/Consolas/CONSOLA.ttf" , 18);
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



    void ImGuiSystem::OnEditorUIRender(float deltaTime) noexcept {
        //static bool showDemo = true;
        //ImGui::ShowDemoWindow(&showDemo);
    }

    void ImGuiSystem::OnDetach(void) noexcept {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }


    void ImGuiSystem::Begin(void) noexcept {
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

    void ImGuiSystem::End(void) noexcept {
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
