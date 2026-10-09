#include "../mspch.h"
#include "Application.hpp"
namespace MultiStation {

	void Application::PushLayer(Layer* layer) noexcept {
		MS_ASSERT(isInitialized, "Application not initiallized");
		m_layers.PushLayer(layer);
		
	}
	void Application::PushOverlay(Layer* layer) noexcept {
		MS_ASSERT(isInitialized, "Application not initiallized");
		m_layers.PushOverlay(layer);
		
	}
	void Application::PopLayer(Layer* layer) noexcept {
		MS_ASSERT(isInitialized, "Application not initiallized");
		m_layers.PopLayer(layer);
	}
	void Application::PopOverlay(Layer* layer) noexcept {
		MS_ASSERT(isInitialized, "Application not initiallized");
		m_layers.PopOverlay(layer);
	}


	












	

}
