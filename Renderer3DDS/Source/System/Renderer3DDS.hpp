#pragma once
#include "../mspch.hpp"

namespace MultiStation {

	class Renderer3DDS : public ISystem {
	public:
		Renderer3DDS(void) noexcept;

		/**
		 * @brief Callback that happens once the system is inserted
		 * at the start of the application
		 *
		 */
		void OnAttach(void) override;

		


		/**
		 * @brief Called on a single thread the main/game thread before the ImGui Render Update
		 * for drawing the game scene
		 *
		 * @param deltaTime The time step from the last frame
		 *
		 */
		void OnUpdate(float deltaTime) override;

		



		/**
		 * @brief Called at the end of the application or at removing the system from the application
		 *
		 * \return
		 */
		void OnDettach(void)  override;

		void OnEvent(Event& e) override;

	};

}

