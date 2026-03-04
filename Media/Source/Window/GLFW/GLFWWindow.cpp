
#include <GLFW.hpp>
#include <Platform.hpp>
#include "GLFWWindow.hpp"
namespace MultiStation {
	uint32_t MS_to_GLFW_key(uint32_t ms_key) noexcept {
		return ms_key; // taken from glfw anyway
	}
	uint32_t GLFW_to_MS_key(uint32_t glfw_key) noexcept {
		return glfw_key; 
	}

	uint32_t MS_to_GLFW_mouse(uint32_t ms_button) noexcept {
		return ms_button;
	}
	uint32_t GLFW_to_MS_mouse(uint32_t glfw_button) noexcept {
		return glfw_button;
	}

	static void MyGLFWErrorCallback(int error, const char* description) {
		MS_ENGINE_ERROR("GLFW : error %d , description %s", error, description);
	}

	GLFWWindow::~GLFWWindow(void) noexcept{
		if (m_NativeWindow) {
			GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
			glfwDestroyWindow(win);
			//glfwTerminate();
		}
	}

	GLFWWindow::GLFWWindow(WindowProperties props) noexcept  {
		m_WinData.Props = props;
		m_NativeWindow = nullptr;

		// set error glfw callback :)
		glfwSetErrorCallback(MyGLFWErrorCallback);
		// Initialise GLFW
		if (!glfwInit())
		{
			MS_ASSERT(0, "GLFW initialisation failed!");
			glfwTerminate();
		}
		// Create the window
		GLFWwindow* mainWindow = glfwCreateWindow(
			m_WinData.Props.Width, m_WinData.Props.Height,
			m_WinData.Props.Title.c_str(),
			NULL, NULL);

		if (!mainWindow)
		{
			MS_ASSERT(0, "GLFW window creation failed!");
			glfwTerminate();
		}
		
		m_NativeWindow = mainWindow;
		glfwMakeContextCurrent(mainWindow);
		glfwSetWindowUserPointer(mainWindow, &m_WinData);


		EventsInit();
		SetVSync(true);
	}

	GLFWWindow::GLFWWindow(GLFWWindow&& win) noexcept : m_WinData() {
		MoveOperation(&win);
	}

	GLFWWindow& GLFWWindow::operator=(GLFWWindow&& win) noexcept {
		MoveOperation(&win);
		return *this;
	}

	GLFWWindow& GLFWWindow::MoveOperation(GLFWWindow* mov) noexcept {
		this->m_NativeWindow = mov->m_NativeWindow;
		this->m_WinData = mov->m_WinData;

		mov->m_WinData.Props = WindowProperties(0, 0, "No Window");
		mov->m_NativeWindow = NULL;
		mov->m_WinData.EventCallback = nullptr;
		return *this;
	}





	const void* GLFWWindow::GetNativeWindow(void) const noexcept{
		return m_NativeWindow;
	}

	void* GLFWWindow::GetNativeWindow(void) noexcept {
		return m_NativeWindow;
	}

	uint16_t GLFWWindow::GetSurfaceWidth(void) const noexcept {
		if (m_NativeWindow == NULL) {
			MS_ASSERT(m_NativeWindow, "No Window Reference , may a move operator was befored called ?");
			return 0;
		}
		
		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		int width, height;
		glfwGetFramebufferSize(win, &width, &height);
		return (uint16_t)width;
	}

	uint16_t GLFWWindow::GetSurfaceHeight(void) const noexcept {
		if (m_NativeWindow == NULL) {
			MS_ASSERT(m_NativeWindow, "No Window Reference , may a move operator was befored called ?");
			return 0;
		}

		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		int width, height;
		glfwGetFramebufferSize(win, &width, &height);
		return (uint16_t)height;
	}

	uint16_t GLFWWindow::GetHeight(void) const noexcept {
		if (m_NativeWindow == NULL) {
			MS_ASSERT(m_NativeWindow, "No Window Reference , may a move operator was befored called ?");
			return 0;
		}
		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		int width, height;
		glfwGetWindowSize(win, &width, &height);
		return (uint16_t)height;
	}

	uint16_t GLFWWindow::GetWidth() const noexcept {
		if (m_NativeWindow == NULL) {
			MS_ASSERT(m_NativeWindow, "No Window Reference , may a move operator was befored called ?");
			return 0;
		}
		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		int width, height;
		glfwGetWindowSize(win, &width, &height);
		return (uint16_t)width;
	}

	const std::string& GLFWWindow::GetName(void) const noexcept {
		if (!m_NativeWindow) {
			MS_ASSERT(m_NativeWindow, "No Window Reference , may a move operator was befored called ?");
			return m_WinData.Props.Title;
		}
		return m_WinData.Props.Title;
	}

	
	void GLFWWindow::SetWidth(uint16_t width) noexcept {
		if (!m_NativeWindow) {
			MS_ASSERT(m_NativeWindow, "No Window Reference , may a move operator was befored called ?");
			return;
		}

		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		int w, h;
		w = width;
		h = m_WinData.Props.Height;
		m_WinData.Props.Width = width;
		glfwSetWindowSize(win, w, h);
	}

	void GLFWWindow::SetHeight(uint16_t height) noexcept {
		if (!m_NativeWindow) {
			MS_ASSERT(m_NativeWindow, "No Window Reference , may a move operator was befored called ?");
			return;
		}

		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		int w, h;
		w = m_WinData.Props.Width;
		h = height;
		m_WinData.Props.Height = height;
		glfwSetWindowSize(win, w, h);
	}

	void GLFWWindow::SetName(const std::string& name) noexcept {
		if (!m_NativeWindow) {
			MS_ASSERT(m_NativeWindow, "No Window Reference , may a move operator was befored called ?");
			return;
		}
		

		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;

		m_WinData.Props.Title = name;
		glfwSetWindowTitle(win, name.c_str());

	}










	bool GLFWWindow::ShouldClose(void) const noexcept {
		
		MS_ASSERT(m_NativeWindow, "No Window Reference , may a move operator was befored called ?");
			
		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		return glfwWindowShouldClose(win);
	}

	void GLFWWindow::PollEvents(void) noexcept {
		
		MS_ASSERT(m_NativeWindow, "No Window Reference , may a move operator was befored called ?");
			
		glfwPollEvents();
	}

	void GLFWWindow::SwapBuffers(void) noexcept {
		
		MS_ASSERT(m_NativeWindow, "No Window Reference , may a move operator was befored called ?");
			
		GLFWwindow* win = (GLFWwindow*)m_NativeWindow;
		glfwSwapBuffers(win);
	}

	
	bool GLFWWindow::IsVSync(void) const noexcept {
		return m_WinData.Props.IsVSync;
	}

	void GLFWWindow::SetVSync(bool vsync) noexcept {
		m_WinData.Props.IsVSync = vsync;
		if (vsync) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}
	}

	void GLFWWindow::OnUpdate(void) noexcept {
		SwapBuffers();
		PollEvents();

	}

	void GLFWWindow::SetEventCallBack(const Window::EventCallBack callback) noexcept {
		m_WinData.EventCallback = callback;
	}
}
