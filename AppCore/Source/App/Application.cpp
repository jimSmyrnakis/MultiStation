#include "Application.hpp"
#include <Platform.hpp>
namespace MultiStation {
	
	Application::Application(void) noexcept {
		m_name = "Application";
		m_systemManager = nullptr;
	}
	Application::~Application(void) noexcept {
		if (m_systemManager) {
			delete m_systemManager;
			m_systemManager = nullptr;
		}
	}
	
	
	void Application::Init(void) noexcept {
		MultiStation::LogInit();
		GetInstance()->m_systemManager = new(std::nothrow) SystemManager();
		if (GetInstance()->m_systemManager == nullptr) {
			MS_ASSERT(false, "Failed to allocate memory for SystemManager.");
			// Handle allocation failure as needed (e.g., throw an exception, set an error state, etc.)
		}
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

}
