#include "GLFWWindow.hpp"
#include <GLFW.hpp>
#include <Platform.hpp>
#include "../../Events/Event.hpp"
#include "../../Events/ApplicationEvent.hpp"
#include "../../Events/KeyEvent.hpp"
#include "../../Events/MouseEvent.hpp"
#include "../../Events/WindowEvents.hpp"
namespace MultiStation {

	void GLFWWindow::EventsInit(void) noexcept {
		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		
		// Window Resize call back
		glfwSetWindowSizeCallback(win, [](GLFWwindow* window, int width, int height) {

				WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
				data.Props.Width = width;
				data.Props.Height = height;
				WindowResizeEvent event(width , height);
				data.EventCallback(event);

			});

		// Window Close call back
		glfwSetWindowCloseCallback(win, [](GLFWwindow* window) {

			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);

			});

		// Window Lose Focus call back
		glfwSetWindowFocusCallback(win, [](GLFWwindow* window, int focused) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowFocusEvent e((focused == GLFW_TRUE) ? true : false);
			data.EventCallback(e);
			});

		// Key Event call back
		glfwSetKeyCallback(win, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			key = GLFW_to_MS_key(key);
			switch (action) {
			case GLFW_RELEASE: { KeyReleasedEvent e(key); data.EventCallback(e); }
								break;
			case GLFW_REPEAT: { KeyPressedEvent e(key, 1); data.EventCallback(e); }
								break;
			case GLFW_PRESS: { KeyPressedEvent e(key, 0); data.EventCallback(e); }
								break;
			default:
				MS_ENGINE_WARN("Unkown Key event !");
			};

			});

		// Key Typed call back
		glfwSetCharCallback(win, [](GLFWwindow* window, unsigned int codepoint) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent e(codepoint);
			data.EventCallback(e);
			});
		// Mouse Button Call back
		glfwSetMouseButtonCallback(win, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			button = GLFW_to_MS_mouse(button);
			switch (action) {
			case GLFW_PRESS: {
					MouseButtonPressedEvent e( button );
					data.EventCallback(e);
					break;
				};
			case GLFW_RELEASE: {
					MouseButtonReleasedEvent e((button));
					data.EventCallback(e);
					break;
				};
			default:
				MS_ENGINE_WARN("Unknown Mouse Button Event !");

			}


			});

		// Mouse Scroll call back 
		glfwSetScrollCallback(win, [](GLFWwindow* window, double xoffset, double yoffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent e(xoffset, yoffset);
			data.EventCallback(e);
			});

		// Mouse move call back 
		glfwSetCursorPosCallback(win, [](GLFWwindow* window, double xpos, double ypos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent e(xpos, ypos);
			data.EventCallback(e);
			});

		// 
	}

}
