#pragma once
#include "../Globals/Globals.hpp"
#include <Media.hpp>
#include <Platform.hpp>
/**
 * @author Dimitris Smyrnakis
 * @file ISystem.hpp
 * @brief This is where the class for the system implamentation interface is .
 * @version 0.1
 */
namespace MultiStation{

	/**
	 * @class ISystem
	 * @brief Contains virtual methods for the system developer to implement 
	 * and simple mechanisms for handling enable , disable states and  providing 
	 * fixed delta time for OnFixedUpdate .  
	 */
	class ISystem {
	public: 

		virtual ~ISystem(void) = default;

		ISystem(void) noexcept ;

		/**
		 * @brief Virtual method that executes when the system is first attached to the engine 
		 */
		virtual void OnAttach(void) noexcept ;

		/**
		 * @brief Virtual method that executes when the system comes from disable to enable state or
		 * after the first OnAttach call . 
		 */
		virtual void OnEnable(void) noexcept ;

		/**
		 * @brief Virtual method that executes every frame (engine loop)  .
		 *
		 * @param delta_time The time step from the last time it was executed (last frame time difference)
		 */
		virtual void OnUpdate(float delta_time) noexcept ;

		/**
		 * @brief Virtual method executes after disabling the system or before OnDettach method . 
		 */
		virtual void OnDisable(void) noexcept ;

		/**
		 * @brief Virtual method that executes when the system is dettached from the engine 
		 * or as otherwise called on the shutdown of the system .
		 */
		virtual void OnDettach(void) noexcept ;

		
		


	public: // Methods to use for the system enable/disable , fixed dt and other info 

		void SetEnable(bool enable);

		bool IsEnable(void) const;

		bool SetFixedDeltaTime(float deltaTime);

		float GetFixedDeltaTime(void) const;

		void SetName(const std::string& name);

		const std::string& GetName(void) const;


	private:
		bool m_enable;
		float m_fixedDeltaTime;
		std::string m_name;

	};

}
