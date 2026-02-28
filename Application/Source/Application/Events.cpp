#include "../mspch.h"
#include "Application.hpp"

namespace MultiStation {

	void Application::OnEvent(Event& e) noexcept {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowCloseEvent));
		//MS_ENGINE_INFO("Event Log : %s", e.ToString().c_str());
		for (auto it = m_systemStack.end(); it != m_systemStack.begin(); ) {
			(*(--it))->OnEvent(e);
			if (e.Handled) break;
		}
	}

	bool Application::OnWindowCloseEvent(WindowCloseEvent& e) noexcept {
		m_isRunning.store(false, std::memory_order_relaxed);
		return true;
	}
}
