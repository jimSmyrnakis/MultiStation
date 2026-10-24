#include "SystemManager.hpp"
#include "time.h"
namespace MultiStation {

	SystemManager::SystemManager(EngineContext& context) noexcept : m_context(context)  {
		
	}

	SystemManager::~SystemManager(void) noexcept {
		for (int i = m_modules.size() - 1; i >= 0; i--) {
			ISystemModule* module = m_modules[i];
			if (!module) continue;

			ISystem& sys = m_modules[i]->GetSystem();
			bool state = m_moduleSystemsState[i];

			if (state) sys.OnDisable();
			m_moduleSystemsState[i] = false;

			

			sys.OnDettach();
			module->Fini(m_context);
			
			
			delete module;
		}
	}



	

	void SystemManager::OnUpdate(float dt) noexcept {
		
		// For each systems state change 
		for (int i = 0; i < m_modules.size(); i++) {
			ISystem& sys = m_modules[i]->GetSystem();
			bool previus_state = m_moduleSystemsState[i];


			if (previus_state != sys.IsEnable()) {
				if (sys.IsEnable()) {
					sys.OnEnable();
					m_moduleSystemsState[i] = true;
				}
				else {
					sys.OnDisable();
					m_moduleSystemsState[i] = false;
				}
					
				

			}

		}


		// For each frame update (When multithreading job system
		// support come this can have phases of parallel systems
		// updates callbacks using jobs now is single threaded )
		for (int i = 0; i < m_modules.size(); i++) {
			ISystem& sys = m_modules[i]->GetSystem();
			bool previus_state = m_moduleSystemsState[i];
			if (m_moduleSystemsState[i])
				sys.OnUpdate( dt);
		}


		


	}


	

}
