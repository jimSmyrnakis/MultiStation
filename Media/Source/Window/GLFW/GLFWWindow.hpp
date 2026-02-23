#pragma once
#include "../Window.hpp"

namespace MultiStation {

	class GLFWWindow : public Window {

	public:
		GLFWWindow(WindowProperties props = WindowProperties()) noexcept;
		~GLFWWindow(void) noexcept;
		
		GLFWWindow(const GLFWWindow& othr) noexcept = delete;
		GLFWWindow& operator=(const GLFWWindow& other) noexcept = delete;

		GLFWWindow(GLFWWindow&& other) noexcept ;
		GLFWWindow& operator=(GLFWWindow&& other) noexcept ;


	public:
		virtual void* GetNativeWindow(void) const noexcept override;

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
