#include "Editor.hpp"
namespace MultiStation {

	Editor::Editor(void) noexcept : IMSSystem("Editor") {

	}

	Editor::~Editor(void) noexcept {

	}


	/**
		 * @brief Callback that happens once the system is inserted
		 * at the start of the application
		 *
		 */
	void Editor::OnAttach(void) noexcept {
		MS_INFO("Editor On Attach");
	}

	bool OnKeyPressedEvent(KeyPressedEvent& e) noexcept {
		if (e.GetKeyCode() == MS_KEY_X) {
			//Application::Get().SetRunning(false);
			Application::Get().GetScene().CreateGameObject("Haha name");
		}
		return false;
	}

	/**
	 *
	 * @brief Called on a single thread the main/game thread before the Render Update .
	 * @param e The event that have been received and parse to us for check
	 *
	 */
	void Editor::OnEvent(Event& e) noexcept {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(OnKeyPressedEvent);

	}

    void SetUnityDarkStyle()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        ImGui::StyleColorsDark();

        // Layout
        style.WindowPadding = ImVec2(8, 8);
        style.FramePadding = ImVec2(5, 3);
        style.ItemSpacing = ImVec2(6, 4);
        style.ItemInnerSpacing = ImVec2(4, 4);
        style.IndentSpacing = 14.0f;

        // Sizes
        style.ScrollbarSize = 14.0f;
        style.GrabMinSize = 10.0f;

        // Rounding (Unity έχει σχεδόν καθόλου)
        style.WindowRounding = 2.0f;
        style.FrameRounding = 2.0f;
        style.PopupRounding = 2.0f;
        style.ScrollbarRounding = 2.0f;
        style.GrabRounding = 2.0f;
        style.TabRounding = 2.0f;

        // Borders
        style.WindowBorderSize = 1.0f;
        style.FrameBorderSize = 0.0f;
        style.PopupBorderSize = 1.0f;

        ImVec4* colors = style.Colors;

        // Text
        colors[ImGuiCol_Text] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

        // Windows
        colors[ImGuiCol_WindowBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);

        // Borders
        colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);

        // Frames
        colors[ImGuiCol_FrameBg] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);

        // Menu
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

        // Scrollbar
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);

        // Check / slider
        colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);

        // Buttons
        colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);

        // Headers (hierarchy selection)
        colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);

        // Separator
        colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);

        // Resize
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);

        // Tabs (dockspace)
        colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.23f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

        // Text selection
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);

        // Drag drop
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);

        // Navigation
        colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    }

	/**
	 * @brief Called each frame / game loop to Update the Imgui UI Render , such as creating buttons
	 * etc. This one is called only from the main/game thread only after the Render Update
	 *
	 * @param deltaTime The time step from the previus imgui call of the previus frame
	 *
	 */
	void Editor::OnEditorUIRender(float deltaTime) noexcept {
		SetUnityDarkStyle();
		//ImGui::ShowStyleEditor();
		m_HierarchyPanel.OnImGuiRender();
	}



	/**
	 * @brief Called at the end of the application or at removing the system from the application
	 *
	 * \return
	 */
	void Editor::OnDetach(void) noexcept {
		MS_INFO("Editor On Detach");
		
	}

}
