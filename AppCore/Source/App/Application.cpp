#include "Application.hpp"
#include <Platform.hpp>
#include "../Layers/ImGuiLayer/ImGuiLayer.hpp"
namespace MultiStation {
	
	Application::Application(void) noexcept {
		
		m_systemManager = new(std::nothrow) SystemManager();
		if (m_systemManager == nullptr) {
			MS_ASSERT(false, "Failed to allocate memory for SystemManager.");
			// Handle allocation failure as needed (e.g., throw an exception, set an error state, etc.)
		}
		m_window = Window::CreateWindow();
		MS_ASSERT(m_window, "Failed to allocate memory for Window");
		m_window->SetName("Window");
		m_window->SwapBuffers();
		m_window->SetEventCallBack(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		m_isRunning.store(true, std::memory_order_relaxed);

		PushOverlay(new ImguiLayer);
	}

	Application::~Application(void) noexcept {
		if (m_systemManager) {
			delete m_systemManager;
			m_systemManager = nullptr;
		}
	}
	
	void Application::Run(void) noexcept {
		Application::OnStart(this);

		while (IsRunning()) {
			m_window->OnUpdate();
			for (auto it = m_layerStack.end(); it != m_layerStack.begin(); ) {
				(*(--it))->OnUpdate(0.016f);
			}
			Application::OnUpdate(this);
			
		}

		Application::OnLeave(this);
	}


	Application* Application::GetInstance(void) noexcept {
		static Application instance;
		return &instance;
	}


	void Application::SetName(const std::string& name) noexcept {
		m_name = name;
	}

	const std::string& Application::GetName(void) const noexcept {
		return m_name;
	}


	SystemManager* Application::GetSystemManager(void) noexcept {
		return m_systemManager;
	}

	const SystemManager* Application::GetSystemManager(void) const noexcept {
		return m_systemManager;
	}

	bool Application::IsRunning(void) const noexcept {
		return m_isRunning.load(std::memory_order_relaxed);
	}

	void Application::SetRunning(bool isRunning) noexcept {
		m_isRunning.store(isRunning, std::memory_order_relaxed);
	}

	Window* Application::GetWindow(void) noexcept {
		return m_window;
	}

	const Window* Application::GetWindow(void) const noexcept {
		return m_window;
	}


	void Application::PushLayer(Layer* layer)noexcept {
		m_layerStack.PushLayer(layer);
	}
	void Application::PushOverlay(Layer* overlay)noexcept {
		m_layerStack.PushOverlay(overlay);
	}
	void Application::PopLayer(Layer* layer)noexcept {
		m_layerStack.PopLayer(layer);
	}
	void Application::PopOverlay(Layer* overlay)noexcept {
		m_layerStack.PopOverlay(overlay);
	}

}
