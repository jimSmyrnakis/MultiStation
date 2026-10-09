#pragma once
#include <stdint.h>
#include <stddef.h>
#include <Platform.hpp>
#include <string>
#include <functional>
#include <memory>
#include "WindowProperties.hpp"
#include "../Events/Event.hpp"
namespace MultiStation {


	

	class Window {

	protected:
		Window(void) noexcept = default;
		virtual ~Window(void) noexcept = default;
		Window& operator=(const Window& win) = delete;
		virtual Window& operator=(Window&& win) noexcept = default;

	public:
		using EventCallBack = std::function<void(Event&)>;

		/**
		 * @returns The Native Window Handler to a pointer void 
		 */
		virtual void* GetNativeWindow(void) noexcept = 0;

		/**
		 * @returns The Native Window Handler to a const pointer void
		 */
		virtual const void* GetNativeWindow(void) const noexcept = 0;

		/**
		 * @return A virtual method that return the surface draw width
		 */
		virtual uint16_t GetSurfaceWidth(void)		const noexcept = 0;

		/**
		 * @return A virtual method that return the surface draw height
		 */
		virtual uint16_t GetSurfaceHeight(void)		const noexcept = 0;

		/**
		 * @return A virtual method that return the window width
		 */
		virtual uint16_t GetWidth(void)				const noexcept = 0;

		/**
		 * @return A virtual method that return the window height
		 */
		virtual uint16_t GetHeight(void)			const noexcept = 0;

		/**
		 * @return A virtual method that return the name of the window . 
		 */
		virtual const std::string& GetName(void)	const noexcept = 0;

		/**
		 * @param width The width of the window
		 * @brief Sets the new Window width
		 */
		virtual void SetWidth(uint16_t width)			noexcept = 0;

		/**
		 * @param height The Height of the window
		 * @brief Sets the new Window height
		 */
		virtual void SetHeight(uint16_t height)			noexcept = 0;

		/**
		 * @param name The name of the window
		 * @brief Sets the new Window name
		 */
		virtual void SetName(const std::string& name)	noexcept = 0;
		


		/**
		 * @returns True if the window should close otherwise false 
		 */
		virtual bool ShouldClose(void)	const noexcept = 0;

		/**
		 * @brief Poll the os window events 
		 */
		virtual void PollEvents(void)	noexcept = 0;

		/**
		 * @brief Swaps the frame buffers of the window 
		 */
		virtual void SwapBuffers(void)	noexcept = 0;

		/**
		 * @returns True if the window is vsync with the refresh rate of the monitor 
		 */
		virtual bool IsVSync(void) const noexcept = 0;

		/**
		 * @param vsync true for enable , false for disable vsync
		 * @brief Set vsync enable or disable
		 */
		virtual void SetVSync(bool vsync) noexcept = 0;

		/**
		 * @brief Not clear yet.
		 */
		virtual void OnUpdate(void) noexcept = 0;
		virtual void SetEventCallBack(const EventCallBack callback) noexcept = 0;


		static Window* CreateWindow(WindowProperties props = WindowProperties());
		static void DestroyWindow(Window** winptr) noexcept;

	protected:
		
	};
}
