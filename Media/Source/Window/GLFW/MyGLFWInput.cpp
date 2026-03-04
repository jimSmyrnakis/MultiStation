#include "MyGLFWInput.hpp"
#include <GLFW.hpp>

namespace MultiStation {


	GLFWInput::GLFWInput(const Window& win) noexcept {
		m_Window = (void*)win.GetNativeWindow();
	}

	GLFWInput::~GLFWInput(void) noexcept {

	}
	
	GLFWInput::GLFWInput(GLFWInput&& other) noexcept {
		this->m_Window = other.m_Window;
	}
	
	GLFWInput& GLFWInput::operator=(GLFWInput&& other) noexcept {
		this->m_Window = other.m_Window;
		return *this;
	}

	bool GLFWInput::IsKeyDownImpl(uint32_t key) noexcept {

		GLFWwindow* win = (GLFWwindow*)m_Window;
		int state = glfwGetKey(win, key);

		return (state == (GLFW_PRESS | GLFW_REPEAT));
	}

	bool GLFWInput::IsKeyUpImpl(uint32_t key) noexcept {
		GLFWwindow* win = (GLFWwindow*)m_Window;
		int state = glfwGetKey(win, key);

		return state == GLFW_RELEASE;
	}

	bool GLFWInput::IsKeyPressedImpl(uint32_t key)				noexcept {
		GLFWwindow* win = (GLFWwindow*)m_Window;
		int state = glfwGetKey(win, key);

		return state == GLFW_PRESS;
	}

	bool GLFWInput::IsMouseButtonDownImpl(uint32_t button)		noexcept {
		GLFWwindow* win = (GLFWwindow*)m_Window;
		int state = glfwGetMouseButton(win, button);
		return state == GLFW_PRESS;
	}

	bool GLFWInput::IsMouseButtonUpImpl(uint32_t button)		noexcept {
		GLFWwindow* win = (GLFWwindow*)m_Window;
		int state = glfwGetMouseButton(win, button);
		return state == GLFW_RELEASE;
	}


	float GLFWInput::GetMouseXImpl(void) noexcept {
		GLFWwindow* win = (GLFWwindow*)m_Window;
		double xpos, ypos;
		glfwGetCursorPos(win, &xpos, &ypos);
		return xpos;
	}

	float GLFWInput::GetMouseYImpl(void) noexcept {
		GLFWwindow* win = (GLFWwindow*)m_Window;
		double xpos, ypos;
		glfwGetCursorPos(win, &xpos, &ypos);
		return ypos;
	}


}
