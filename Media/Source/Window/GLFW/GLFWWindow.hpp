#pragma once
#include "../Window.hpp"
#include "../../KeyCodes.hpp"

namespace MultiStation {

	uint32_t MS_to_GLFW_key(uint32_t ms_key) noexcept;
	uint32_t GLFW_to_MS_key(uint32_t glfw_key) noexcept;

	uint32_t MS_to_GLFW_mouse(uint32_t ms_button) noexcept;
	uint32_t GLFW_to_MS_mouse(uint32_t glfw_button) noexcept;
	class GLFWWindow : public Window {

	public:
		GLFWWindow(WindowProperties props = WindowProperties()) noexcept;
		~GLFWWindow(void) noexcept;
		
		GLFWWindow(const GLFWWindow& othr) noexcept = delete;
		GLFWWindow& operator=(const GLFWWindow& other) noexcept = delete;

		GLFWWindow(GLFWWindow&& other) noexcept ;
		GLFWWindow& operator=(GLFWWindow&& other) noexcept ;


	public:
		virtual void* GetNativeWindow(void) noexcept override;
		virtual const void* GetNativeWindow(void) const noexcept override;

		virtual uint16_t GetSurfaceWidth(void)		const noexcept override;
		virtual uint16_t GetSurfaceHeight(void)		const noexcept override;
		virtual uint16_t GetWidth(void)				const noexcept override;
		virtual uint16_t GetHeight(void)			const noexcept override;
		virtual const std::string& GetName(void)	const noexcept override;

		virtual void SetWidth(uint16_t width)			noexcept override;
		virtual void SetHeight(uint16_t height)			noexcept override;
		virtual void SetName(const std::string& name)	noexcept override;




		virtual bool ShouldClose(void)	const noexcept override;
		virtual void PollEvents(void)	noexcept override;
		virtual void SwapBuffers(void)	noexcept override;
		virtual bool IsVSync(void) const noexcept override;
		virtual void SetVSync(bool vsync) noexcept override;
		virtual void OnUpdate(void) noexcept override;
		virtual void SetEventCallBack(const Window::EventCallBack callback) noexcept override;
	private:
		GLFWWindow& MoveOperation(GLFWWindow* mov) noexcept;
		void EventsInit(void) noexcept;
	private:

		void* m_NativeWindow;

		struct WindowData {
			WindowProperties Props;
			Window::EventCallBack EventCallback;
			bool IsVSync;

			
		};

		WindowData m_WinData;
	};

}
