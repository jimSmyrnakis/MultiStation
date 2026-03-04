#pragma once
#include "../../mspch.hpp"

namespace MultiStation {

	class Renderer3DDS : public ISystem {
	public:
		Renderer3DDS(void) noexcept;

		/**
		 * @brief Callback that happens once the system is inserted
		 * at the start of the application
		 *
		 */
		virtual void OnAttach(void) noexcept override;

		


		/**
		 * @brief Called on a single thread the main/game thread before the ImGui Render Update
		 * for drawing the game scene
		 *
		 * @param deltaTime The time step from the last frame
		 *
		 */
		virtual void OnRenderUpdate(float deltaTime) noexcept override;

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

	};

}

