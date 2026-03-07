#include "../mspch.h"
#include "Application.hpp"
namespace MultiStation {

	void Application::PushSystemLayer(IMSSystem* system) noexcept {
		MS_ASSERT(isInitialized, "Application not initiallized");
		m_systemStack.PushLayer(system);
		AddSystem(system, SystemFlags::SYSTEM_ON_LAYER_MANAGER);
	}
	void Application::PushSystemOverlay(IMSSystem* system) noexcept {
		MS_ASSERT(isInitialized, "Application not initiallized");
		m_systemStack.PushOverlay(system);
		AddSystem(system, SystemFlags::SYSTEM_ON_LAYER_MANAGER);
	}
	void Application::PopSystemLayer(IMSSystem* system) noexcept {
		MS_ASSERT(isInitialized, "Application not initiallized");
		m_systemStack.PopLayer(system);
		RemoveSystem(system, SystemFlags::SYSTEM_ON_LAYER_MANAGER);
	}
	void Application::PopSystemOverlay(IMSSystem* system) noexcept {
		MS_ASSERT(isInitialized, "Application not initiallized");
		m_systemStack.PopOverlay(system);
		RemoveSystem(system, SystemFlags::SYSTEM_ON_LAYER_MANAGER);
	}


	void Application::CreatePhase(uint32_t phase) noexcept {
		MS_ASSERT(isInitialized, "Application not initiallized");
		m_systemManager.CreatePhase(phase);
	}
	void Application::DestroyPhase(uint32_t phase) noexcept {
		MS_ASSERT(isInitialized, "Application not initiallized");
		m_systemManager.DestroyPhase(phase);
	}
	void Application::AddSystemToPhase(IMSSystem* system , uint32_t phase) noexcept {
		MS_ASSERT(isInitialized, "Application not initiallized");
		m_systemManager.AddSystem(system , phase);
		AddSystem(system, SystemFlags::SYSTEM_ON_UPDATE_MANAGER);
	}
	void Application::RemoveSystemFromPhase(IMSSystem* system , uint32_t phase) noexcept {
		MS_ASSERT(isInitialized, "Application not initiallized");
		m_systemManager.RemoveSystem(system , phase);
		RemoveSystem(system, SystemFlags::SYSTEM_ON_UPDATE_MANAGER);
	}













	bool Application::IsFlagsField(uint32_t flags) noexcept {
		return  ((flags & SystemFlags::SYSTEM_ON_LAYER_MANAGER) == SystemFlags::SYSTEM_ON_LAYER_MANAGER)
			|| ((flags & SystemFlags::SYSTEM_ON_UPDATE_MANAGER) == SystemFlags::SYSTEM_ON_UPDATE_MANAGER);
	}

	void Application::AddSystem(IMSSystem* system, uint32_t flags) noexcept {
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
	void Application::RemoveSystem(IMSSystem* system, uint32_t flags) noexcept {
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
