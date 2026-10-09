#include "SystemManager.hpp"
#include "time.h"
namespace MultiStation {

	SystemManager::SystemManager(EngineContext& context) : m_context(context) {
		
	}

	SystemManager::~SystemManager(void) {
		for (ISystemModule* module : m_modules) {
			if (!module) continue;
			
			module->GetSystem().OnDettach();
			module->Fini(m_context);
			
			
			delete module;
		}
	}



	

	void SystemManager::OnUpdate(float dt) {
		

		for (ISystemModule* module : m_modules) {
			module->GetSystem().OnUpdate( dt);
		}
	}


	
	void SystemManager::OnEvent(Event& e) {
		for (ISystemModule* module : m_modules) {
			module->GetSystem().OnEvent(e);
		}
	}

}
