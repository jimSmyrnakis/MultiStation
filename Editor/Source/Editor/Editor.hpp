#pragma once
#include <ECS.hpp>
#include "../HierarchyPanel/HierarchyPanel.hpp"
namespace MultiStation {
	class Editor : public ISystem {
	public:
		Editor(void) noexcept;
		~Editor(void) noexcept;
		Editor(const Editor&) noexcept = delete;
		Editor& operator=(const Editor&) noexcept = delete;
		Editor(Editor&&) noexcept = delete;
		Editor& operator=(Editor&&) noexcept = delete;

		/**
		 * @brief Callback that happens once the system is inserted
		 * at the start of the application
		 *
		 */
		virtual void OnAttach(void) noexcept override;



		/**
		 *
		 * @brief Called on a single thread the main/game thread before the Render Update .
		 * @param e The event that have been received and parse to us for check
		 *
		 */
		virtual void OnEvent(Event& e) noexcept override;


		/**
		 * @brief Called each frame / game loop to Update the Imgui UI Render , such as creating buttons
		 * etc. This one is called only from the main/game thread only after the Render Update
		 *
		 * @param deltaTime The time step from the previus imgui call of the previus frame
		 *
		 */
		virtual void OnImGuiRender(float deltaTime) noexcept override;



		/**
		 * @brief Called at the end of the application or at removing the system from the application
		 *
		 * \return
		 */
		virtual void OnDetach(void) noexcept override;


	private:
		HierarchyPanel m_HierarchyPanel;

	};
}	
