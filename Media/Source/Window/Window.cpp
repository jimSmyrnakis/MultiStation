#include "Window.hpp"
#include "GLFW/GLFWWindow.hpp"
namespace MultiStation {

	Window* Window::CreateWindow(WindowProperties props) {
		Window* win = new(std::nothrow) GLFWWindow(props);
		MS_ASSERT(win, "Failed to allocate memory for the Window !");
		return win;
	}

	void Window::DestroyWindow(Window** winptr) noexcept {
		delete (*winptr);
		(*winptr) = nullptr;
	}

}
