#define GLFW_INCLUDE_NONE
#define GLFW_STATIC
#include <GLFW/glfw3.h>
#include "../Window.hpp"

namespace MultiStation {
	Window::~Window(void) noexcept{
		if (m_NativeWindow) {
			GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
			glfwDestroyWindow(win);
			glfwTerminate();
		}
	}
	Window::Window(void) noexcept {
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

	Window::Window(Window&& win) noexcept {
		MoveOperation(&win);
	}
	Window& Window::operator=(Window&& win) noexcept {
		MoveOperation(&win);
		return *this;
	}
	Window& Window::MoveOperation(Window* mov) noexcept {
		this->m_Height = mov->m_Height;
		this->m_Width = mov->m_Width;
		this->m_NativeWindow = mov->m_NativeWindow;

		this->m_Height = 0;
		this->m_Width = 0;
		this->m_NativeWindow = NULL;
		return *this;
	}





	void* Window::GetNativeWindow() const {
		return m_NativeWindow;
	}

	uint16_t Window::GetSurfaceWidth() const {
		if (m_NativeWindow == NULL) {
			ASSERT(m_NativeWindow, "No Window Reference , may a move operator was befored called ?");
			return 0;
		}
		
		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		int width, height;
		glfwGetFramebufferSize(win, &width, &height);
		return (uint16_t)width;
	}

	uint16_t Window::GetSurfaceHeight() const {
		if (m_NativeWindow == NULL) {
			ASSERT(m_NativeWindow, "No Window Reference , may a move operator was befored called ?");
			return 0;
		}

		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		int width, height;
		glfwGetFramebufferSize(win, &width, &height);
		return (uint16_t)height;
	}

	uint16_t Window::GetHeight() const {
		if (m_NativeWindow == NULL) {
			ASSERT(m_NativeWindow, "No Window Reference , may a move operator was befored called ?");
			return 0;
		}
		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		int width, height;
		glfwGetWindowSize(win, &width, &height);
		return (uint16_t)height;
	}

	uint16_t Window::GetWidth() const {
		if (m_NativeWindow == NULL) {
			ASSERT(m_NativeWindow, "No Window Reference , may a move operator was befored called ?");
			return 0;
		}
		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		int width, height;
		glfwGetWindowSize(win, &width, &height);
		return (uint16_t)width;
	}
	const std::string& Window::GetName(void) const {
		if (!m_NativeWindow) {
			ASSERT(m_NativeWindow, "No Window Reference , may a move operator was befored called ?");
			return m_Name;
		}
		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		const char* name = glfwGetWindowTitle(win);
		m_Name = std::move(std::string(name));
		
		return m_Name;
	}

	
	void Window::SetWidth(uint16_t width) {
		if (!m_NativeWindow) {
			ASSERT(m_NativeWindow, "No Window Reference , may a move operator was befored called ?");
			return;
		}

		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		int w, h;
		w = width;
		h = m_Height;

		glfwSetWindowSize(win, w, h);
	}
	void Window::SetHeight(uint16_t height) {
		if (!m_NativeWindow) {
			ASSERT(m_NativeWindow, "No Window Reference , may a move operator was befored called ?");
			return;
		}

		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		int w, h;
		w = m_Width;
		h = height;

		glfwSetWindowSize(win, w, h);
	}
	void Window::SetName(const std::string& name) {
		if (!m_NativeWindow) {
			ASSERT(m_NativeWindow, "No Window Reference , may a move operator was befored called ?");
			return;
		}
		

		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;

		m_Name = name;
		glfwSetWindowTitle(win, m_Name.c_str());

	}










	bool Window::ShouldClose() const {
		
		ASSERT(m_NativeWindow, "No Window Reference , may a move operator was befored called ?");
			
		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		return glfwWindowShouldClose(win);
	}

	void Window::PollEvents() {
		
		ASSERT(m_NativeWindow, "No Window Reference , may a move operator was befored called ?");
			
		glfwPollEvents();
	}

	void Window::SwapBuffers() {
		
		ASSERT(m_NativeWindow, "No Window Reference , may a move operator was befored called ?");
			
		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		glfwSwapBuffers(win);
	}
}
