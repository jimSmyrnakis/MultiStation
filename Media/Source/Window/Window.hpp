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
		Window(const Window& win) = delete;
		Window(Window&& win) noexcept = default;
		Window& operator=(const Window& win) = delete;
		virtual Window& operator=(Window&& win) noexcept = default;

	public:
		using EventCallBack = std::function<void(Event&)>;

		virtual void* GetNativeWindow(void) noexcept = 0;
		virtual const void* GetNativeWindow(void) const noexcept = 0;

		virtual uint16_t GetSurfaceWidth(void)		const noexcept = 0;
		virtual uint16_t GetSurfaceHeight(void)		const noexcept = 0;
		virtual uint16_t GetWidth(void)				const noexcept = 0;
		virtual uint16_t GetHeight(void)			const noexcept = 0;
		virtual const std::string& GetName(void)	const noexcept = 0;

		virtual void SetWidth(uint16_t width)			noexcept = 0;
		virtual void SetHeight(uint16_t height)			noexcept = 0;
		virtual void SetName(const std::string& name)	noexcept = 0;
		



		virtual bool ShouldClose(void)	const noexcept = 0;
		virtual void PollEvents(void)	noexcept = 0;
		virtual void SwapBuffers(void)	noexcept = 0;
		virtual bool IsVSync(void) const noexcept = 0;
		virtual void SetVSync(bool vsync) noexcept = 0;
		virtual void OnUpdate(void) noexcept = 0;
		virtual void SetEventCallBack(const EventCallBack callback) noexcept = 0;


		static Window* CreateWindow(WindowProperties props = WindowProperties());
		static void DestroyWindow(Window** winptr) noexcept;

	protected:
		
	};
}
