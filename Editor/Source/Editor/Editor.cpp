#include "Editor.hpp"
namespace MultiStation {

	Editor::Editor(void) noexcept : ISystem("Editor") {

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



	/**
	 *
	 * @brief Called on a single thread the main/game thread before the Render Update .
	 * @param e The event that have been received and parse to us for check
	 *
	 */
	void Editor::OnEvent(Event& e) noexcept {

	}


	/**
	 * @brief Called each frame / game loop to Update the Imgui UI Render , such as creating buttons
	 * etc. This one is called only from the main/game thread only after the Render Update
	 *
	 * @param deltaTime The time step from the previus imgui call of the previus frame
	 *
	 */
	void Editor::OnImGuiRender(float deltaTime) noexcept {
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
