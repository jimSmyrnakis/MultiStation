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

	

	/**
	 * @brief Called each frame / game loop to Update the Imgui UI Render , such as creating buttons
	 * etc. This one is called only from the main/game thread only after the Render Update
	 *
	 * @param deltaTime The time step from the previus imgui call of the previus frame
	 *
	 */
	void Editor::OnEditorUIRender(float deltaTime) noexcept {
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
