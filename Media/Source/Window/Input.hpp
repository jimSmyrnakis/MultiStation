#pragma once
#include "../KeyCodes.hpp"
#include "Window.hpp"
/**
 * @author Dimitris Smyrnakis
 * @file Input.hpp 
 * @class Input
 * @brief Gives the state of the input by polling
 */
namespace MultiStation {

	class Input {

	public:
		
		/**
		 * 
		 * @param key ms key keycode 
		 * @return true if key is down/pressed but haven't released yet 
		 */
		static bool IsKeyDown(uint32_t key) noexcept;

		/**
		 * .
		 * 
		 * @param key ms key keycode
		 * @return true if key is released 
		 */
		static bool IsKeyUp(uint32_t key) noexcept;

		/**
		 * .
		 *
		 * @param key ms key keycode
		 * @return true if key is pressed this time
		 */
		static bool IsKeyPressed(uint32_t key) noexcept ;

		/**
		 * .
		 *
		 * @param button ms button keycode
		 * @return true if button is pressed
		 */
		static bool IsMouseButtonDown(uint32_t button) noexcept;

		/**
		 * .
		 *
		 * @param button ms button keycode
		 * @return true if button is released
		 */
		static bool IsMouseButtonUp(uint32_t button) noexcept ;

		/**
		 * .
		 *
		 * @return mouse position x on the window
		 */
		static float GetMouseX(void) noexcept ;

		/**
		 * .
		 *
		 * @return mouse position y on the window
		 */
		static float GetMouseY(void) noexcept ;

		/**
		 * @brief Get the Singleton
		 * 
		 * @return Singleton Input
		 */
		static Input* Get(void) noexcept;

		/**
		 * @brief Deletes the singleton , then we can call again the Init
		 * and Get to get a new singleton. Only one instance of Ipnut at a time .
		 * 
		 */
		static void Destroy(void) noexcept;

		/**
		 * @brief Before everything else and after the window creation 
		 * first initialize input by giving the window that the input will 
		 * come from . 
		 * 
		 * @param win The Application Window 
		 *  
		 */
		static void Init(const Window& win) noexcept;

	protected:

		Input(void) noexcept = default;
		virtual ~Input(void) noexcept = default;
		Input(const Input& other) noexcept = delete;
		Input(Input&& other) noexcept = default;
		Input& operator=(const Input& other) noexcept = delete;
		Input& operator=(Input&& other) noexcept = default;

		virtual bool IsKeyDownImpl(uint32_t key) noexcept = 0;
		virtual bool IsKeyUpImpl(uint32_t key) noexcept = 0;
		virtual bool IsKeyPressedImpl(uint32_t key) noexcept = 0;

		virtual bool IsMouseButtonDownImpl(uint32_t button) noexcept = 0;
		virtual bool IsMouseButtonUpImpl(uint32_t button) noexcept = 0;

		virtual float GetMouseXImpl(void) noexcept = 0;
		virtual float GetMouseYImpl(void) noexcept = 0;

		

	private:
		static Input* s_Instance ;
	};


}
