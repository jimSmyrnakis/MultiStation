#define GLFW_INCLUDE_NONE
#define GLFW_STATIC
#include <GLFW/glfw3.h>
#include "../Window.hpp"

namespace MultiStation {
	Window::~Window(void) {
		if (m_NativeWindow) {
			GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
			glfwDestroyWindow(win);
			glfwTerminate();
		}
	}
	Window::Window(void) {
		m_NativeWindow = nullptr;
		// Initialise GLFW
		if (!glfwInit())
		{
			ASSERT(0, "GLFW initialisation failed!");
			glfwTerminate();
		}
		// Create the window
		GLFWwindow* mainWindow = glfwCreateWindow(800, 600, "MultiStation Window", NULL, NULL);
		if (!mainWindow)
		{
			ASSERT(0, "GLFW window creation failed!");
			glfwTerminate();
		}
		m_NativeWindow = (void*)mainWindow;
		glfwMakeContextCurrent(mainWindow);
		m_Height = 600;
		m_Width = 800;

	}
	void* Window::GetNativeWindow() const {
		return m_NativeWindow;
	}

	uint32_t Window::GetSurfaceWidth() const {
		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		int width, height;
		glfwGetFramebufferSize(win, &width, &height);
		return (uint32_t)width;
	}

	uint32_t Window::GetSurfaceHeight() const {
		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		int width, height;
		glfwGetFramebufferSize(win, &width, &height);
		return (uint32_t)height;
	}

	uint32_t Window::GetHeight() const {
		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		int width, height;
		glfwGetWindowSize(win, &width, &height);
		return (uint32_t)height;
	}

	uint32_t Window::GetWidth() const {
		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		int width, height;
		glfwGetWindowSize(win, &width, &height);
		return (uint32_t)width;
	}

	bool Window::ShouldClose() const {
		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		return glfwWindowShouldClose(win);
	}

	void Window::PollEvents() {
		glfwPollEvents();
	}

	void Window::SwapBuffers() {
		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		glfwSwapBuffers(win);
	}
}
