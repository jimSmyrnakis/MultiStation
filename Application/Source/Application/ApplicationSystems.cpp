#include "../mspch.h"
#include "Application.hpp"
namespace MultiStation {

	void Application::PushSystemLayer(ISystem* system) noexcept {
		MS_ASSERT(isInitialized, "Application not initiallized");
		m_systemStack.PushLayer(system);
		AddSystem(system, SystemFlags::SYSTEM_ON_LAYER_MANAGER);
	}
	void Application::PushSystemOverlay(ISystem* system) noexcept {
		MS_ASSERT(isInitialized, "Application not initiallized");
		m_systemStack.PushOverlay(system);
		AddSystem(system, SystemFlags::SYSTEM_ON_LAYER_MANAGER);
	}
	void Application::PopSystemLayer(ISystem* system) noexcept {
		MS_ASSERT(isInitialized, "Application not initiallized");
		m_systemStack.PopLayer(system);
		RemoveSystem(system, SystemFlags::SYSTEM_ON_LAYER_MANAGER);
	}
	void Application::PopSystemOverlay(ISystem* system) noexcept {
		MS_ASSERT(isInitialized, "Application not initiallized");
		m_systemStack.PopOverlay(system);
		RemoveSystem(system, SystemFlags::SYSTEM_ON_LAYER_MANAGER);
	}


	uint32_t Application::CreatePhase(void) noexcept {
		MS_ASSERT(isInitialized, "Application not initiallized");
		return m_systemManager.CreatePhase();
	}
	void Application::BindPhase(uint32_t phase) noexcept {
		MS_ASSERT(isInitialized, "Application not initiallized");
		m_systemManager.BindPhase(phase);
	}
	void Application::AddSystemOnPhase(ISystem* system) noexcept {
		MS_ASSERT(isInitialized, "Application not initiallized");
		m_systemManager.AddSystem(system);
		AddSystem(system, SystemFlags::SYSTEM_ON_UPDATE_MANAGER);
	}
	void Application::RemoveSystemFromPhase(ISystem* system) noexcept {
		MS_ASSERT(isInitialized, "Application not initiallized");
		m_systemManager.RemoveSystem(system);
		RemoveSystem(system, SystemFlags::SYSTEM_ON_UPDATE_MANAGER);
	}













	bool Application::IsFlagsField(uint32_t flags) noexcept {
		return  ((flags & SystemFlags::SYSTEM_ON_LAYER_MANAGER) == SystemFlags::SYSTEM_ON_LAYER_MANAGER)
			|| ((flags & SystemFlags::SYSTEM_ON_UPDATE_MANAGER) == SystemFlags::SYSTEM_ON_UPDATE_MANAGER);
	}

	void Application::AddSystem(ISystem* system, uint32_t flags) noexcept {
		MS_ASSERT(IsFlagsField(flags), "Bad flags!!!");
		auto it = std::find(m_systems.begin(), m_systems.end(), system);
		
		if (it == m_systems.end()) {
			// if you havent that system in the systems list 
			system->OnAttach();
			m_systems.push_back(system);
			m_systemsFlags.push_back(flags);
			return;
		}

		uint32_t index = std::distance(m_systems.begin(), it);
		m_systemsFlags[index] = m_systemsFlags[index] | flags;

	}
	void Application::RemoveSystem(ISystem* system, uint32_t flags) noexcept {
		MS_ASSERT(IsFlagsField(flags), "Bad flags!!!");
		auto it = std::find(m_systems.begin(), m_systems.end(), system);
		bool systemFound = it != m_systems.end();
		if (!systemFound) {
			MS_ENGINE_WARN( "System already removed ");
			return;
		}

		uint32_t index = std::distance(m_systems.begin(), it);
		m_systemsFlags[index] = m_systemsFlags[index] & ~flags;
		if (m_systemsFlags[index] == 0) {
			system->OnDetach();
			m_systems.erase(it);
			auto itFlags = m_systemsFlags.begin() + index;
			m_systemsFlags.erase(itFlags);
			delete system;
		}
		

			
		
	}

}
