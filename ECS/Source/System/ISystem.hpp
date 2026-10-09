#pragma once
#include "../Globals/Globals.hpp"
#include <Media.hpp>
namespace MultiStation{
	extern class Scene;

	class ISystem {
	public:

		virtual ~ISystem(void) = default;

		/**
		 * @brief Virtual method that executes when the system is first attached to the engine 
		 */
		virtual void OnAttach(void) = 0;


		/**
		 * @brief Virtual method that executes when the system is dettached from the engine
		 */
		virtual void OnDettach(void) = 0;

		
		/**
		 * @brief Virtual method that executes every frame (engine loop)  .
		 * 
		 * @param delta_time The time step from the last time it was executed (last frame time difference)
		 */
		virtual void OnUpdate(float delta_time) = 0;

		/**
		 * @brief Virtual method that executes when the system receives an event  .
		 * 
		 * @param e The event 
		 */
		virtual void OnEvent( Event& e) = 0;




	};

}
