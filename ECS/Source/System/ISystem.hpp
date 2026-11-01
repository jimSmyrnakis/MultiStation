#pragma once
#include "../Globals/Globals.hpp"
#include <Media.hpp>
#include <Platform.hpp>
/**
 * @author Dimitris Smyrnakis
 * @file ISystem.hpp
 * @brief System lifecycle callbacks and runtime settings.
 * @version 0.1
 */
namespace MultiStation{

	/**
	 * @class ISystem
	 * @brief Base interface for a module's system, driven by SystemManager.
	 *
	 * After module Init(), the manager calls OnAttach(), then OnEnable().
	 * Each frame it processes enable/disable changes before calling OnUpdate()
	 * on enabled systems, in module registration order. OnEnable() and
	 * OnDisable() may repeat as the requested enabled state changes.
	 *
	 * When the module is removed or the manager shuts down, OnDisable() is
	 * called if the system was last processed as enabled, then OnDettach(),
	 * followed by module Fini(). Destruction alone does not invoke these callbacks.
	 *
	 * @note All lifecycle callbacks have empty default implementations.
	 * @warning Callbacks must not let exceptions escape, add or remove modules,
	 * or recursively update the manager.
	 */
	class ISystem {
	public: 

		/** @brief Destroys the system; lifecycle shutdown is handled by SystemManager. */
		virtual ~ISystem(void) = default;

		/** @brief Starts enabled, named "System", with a fixed delta time of 1/60. */
		ISystem(void) noexcept ;

	public: // Main FSM callbacks
		/**
		 * @brief Called once after module initialization, before the initial OnEnable().
		 */
		virtual void OnAttach(void) noexcept ;

		/**
		 * @brief Called after attachment and when the manager processes re-enabling.
		 * @note The initial call occurs even if IsEnable() is false; the manager
		 * processes that disabled state on its next update.
		 */
		virtual void OnEnable(void) noexcept ;

		/**
		 * @brief Called once per manager update while processed as enabled.
		 * @param[in] delta_time Frame delta time supplied by the manager.
		 * @note Enable changes made during frame updates take effect on the next
		 * manager update, including changes to systems that have not updated yet.
		 */
		virtual void OnUpdate(float delta_time) noexcept ;

		/**
		 * @brief Called when the manager processes disabling, or before detachment
		 * if the system was last processed as enabled.
		 */
		virtual void OnDisable(void) noexcept ;

		/**
		 * @brief Called once on module removal or manager shutdown, before module Fini().
		 */
		virtual void OnDettach(void) noexcept ;

	


		


	public: 

		/**
		 * @brief Requests an enabled state without immediately invoking callbacks.
		 * @param[in] enable Whether the system should receive frame updates.
		 * @note SystemManager processes the flag during its next state-check pass.
		 */
		void SetEnable(bool enable);

		/** @brief Returns the requested enabled state, which may not yet be processed. */
		bool IsEnable(void) const;

		/**
		 * @brief Stores a fixed delta time if it is at least 1/60.
		 * @param[in] deltaTime Requested fixed delta time.
		 * @return true if accepted; false leaves the previous value unchanged.
		 * @note This setting does not schedule updates; SystemManager does not use it.
		 */
		bool SetFixedDeltaTime(float deltaTime);

		/** @brief Returns the stored fixed delta time. */
		float GetFixedDeltaTime(void) const;

		/**
		 * @brief Sets the system's name; it does not affect module identity or update order.
		 * @param[in] name Name to store.
		 */
		void SetName(const std::string& name);

		/** @brief Returns a reference to the system's stored name. */
		const std::string& GetName(void) const;


	private:
		bool m_enable;
		float m_fixedDeltaTime;
		std::string m_name;

	};

}
