#include "../mspch.h"
#include "Application.hpp"

namespace MultiStation {

	void Application::OnEvent(Event& e) noexcept {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowCloseEvent));
		//MS_ENGINE_INFO("Event Log : %s", e.ToString().c_str());
		m_layers.OnEvent(e);
		m_engine.OnEvent(e);
	}

	bool Application::OnWindowCloseEvent(WindowCloseEvent& e) noexcept {
		m_isRunning.store(false, std::memory_order_relaxed);
		return true;
	}
}
