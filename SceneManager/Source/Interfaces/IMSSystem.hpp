#pragma once
#include <stdint.h>
#include <stddef.h>
#include <ECS.hpp>
#include <Media.hpp>

/**
 * @author Dimitris Smyrnakis 
 * @file ISystem.hpp
 * @class ISystem 
 * @brief Provides the interface for someone to make a system for rendering , computing , gui handle 
 * or/and Input Events
 */
namespace MultiStation{

	

	class IMSSystem {
	
	public:
		IMSSystem(const std::string name) noexcept ;
		virtual ~IMSSystem(void) noexcept = default;

		IMSSystem(const IMSSystem& cpy) noexcept = delete;
		IMSSystem& operator=(const IMSSystem& cpy) noexcept = delete;
	public:
		IMSSystem(IMSSystem&& move) noexcept = default;
		virtual IMSSystem& operator=(IMSSystem&& move) noexcept = default ;

		/**
		 * @brief Callback that happens once the system is inserted 
		 * at the start of the application
		 *  
		 */
		virtual void OnAttach(void) noexcept;

		/**
		 * @brief Called every game loop in parallel with the rest systems
		 * in the same phase .
		 * 
		 * @param[in] deltaTime float that provides the time step from the last iteration
		 * 
		 */
		virtual void OnUpdate(float deltaTime) noexcept ;


		/**
		 * 
		 * @brief Called on a single thread the main/game thread before the Render Update . 
		 * @param e The event that have been received and parse to us for check
		 * 
		 */
		virtual void OnEvent(Event& e) noexcept ;

		/**
		 * @brief Called on a single thread the main/game thread before the ImGui Render Update 
		 * for drawing the game scene
		 *
		 * @param deltaTime The time step from the last frame
		 *
		 */
		virtual void OnRenderUpdate(float deltaTime) noexcept;

		/**
		 * @brief Called each frame / game loop to Update the Imgui UI Render , such as creating buttons
		 * etc. This one is called only from the main/game thread only after the Render Update
		 * 
		 * @param deltaTime The time step from the previus imgui call of the previus frame
		 * 
		 */
		virtual void OnEditorUIRender(float deltaTime) noexcept ;

		

		/**
		 * @brief Called at the end of the application or at removing the system from the application
		 * 
		 * \return 
		 */
		virtual void OnDetach(void) noexcept;
		
	public:

		/**
		 * @returns A unique id per ISystem class
		 */
		template<typename SystemType>
		static uint32_t GetTypeID(void);

		std::string GetName(void) noexcept;

		const std::string GetName(void) const noexcept;
		
	private:
		static uint32_t s_typeID;
		std::string m_name;
	};


	template<typename SystemType>
	static uint32_t IMSSystem::GetTypeID(void) {
		// force T to be derived from ISystem , but not ISystem itself
		static_assert(
			std::is_base_of<IMSSystem, SystemType>::value &&
			!std::is_same<IMSSystem, SystemType>::value,
			"T must be derived from ISystem, not ISystem itself!");

		static uint32_t id = ++s_typeID;

		return id;
	}




}
