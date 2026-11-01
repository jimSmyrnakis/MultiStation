#include "SystemManager.hpp"
#include "time.h"
/**
 * @file SystemManager.cpp
 * @brief Deferred module initialization, ordered updates, and reverse teardown.
 */
namespace MultiStation {

	SystemManager::SystemManager(EngineContext& context) noexcept : m_context(context)  {
		
	}

	SystemManager::~SystemManager(void) noexcept {
		FiniModules();
		// FiniModules() normally empties the collection; delete any remaining entries.
		for (auto& module : m_modules){
			if (module)
				delete module;
		}
	}

	void SystemManager::InitModules(void) noexcept {
		for (int i = 0; i < m_modules.size(); i++) {
			ISystemModule* module = m_modules[i];
			if (!module || m_moduleIsInit[i])
				continue;
			
			m_moduleIsInit[i] = true;
			// Initialize module resources before accessing its system.
			module->Init(m_context);

			// Attach before sampling the initial requested enabled state.
			module->GetSystem().OnAttach();

			// Store the initial state; disabled systems receive no enable callback.
			if (m_moduleSystemsState[i] = 
				module->GetSystem().IsEnable())
				module->GetSystem().OnEnable();


		}
	}

	void SystemManager::FiniModules(void) noexcept {
		if (m_modules.size() == 0) return;

		for (int i = m_modules.size() - 1; i >= 0; i--) {
			ISystemModule* module = m_modules[i];
			MS_ASSERT(module != nullptr, "Null module should never happen");

			if (m_moduleIsInit[i]) {
				ISystem& sys = m_modules[i]->GetSystem();
				bool state = m_moduleSystemsState[i];

				if (state) sys.OnDisable();
				m_moduleSystemsState[i] = false;



				sys.OnDettach();
				module->Fini(m_context);
				
			}
			
			delete module;
			// Erase matching indices from every collection to preserve alignment.
			m_modules.erase(m_modules.begin() + i);
			m_moduleIsInit.erase(m_moduleIsInit.begin() + i);
			m_moduleSystemsState.erase(m_moduleSystemsState.begin() + i);
			m_modulesIds.erase(m_modulesIds.begin() + i);
		}
	}



	

	void SystemManager::OnUpdate(float dt) noexcept {
		
		// Process transitions only for initialized modules, before any frame updates.
		for (int i = 0; i < m_modules.size(); i++) {
			
			ISystemModule* module = m_modules[i];
			if (!module || !m_moduleIsInit[i]) continue;

			ISystem& sys = module->GetSystem();
			
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


		// Update systems using the states processed above, in registration order.
		for (int i = 0; i < m_modules.size(); i++) {
			ISystemModule* module = m_modules[i];
			if (!module || !m_moduleIsInit[i]) continue;

			ISystem& sys = module->GetSystem();
			bool previus_state = m_moduleSystemsState[i];
			if (m_moduleSystemsState[i])
				sys.OnUpdate( dt);
		}


		


	}


























	void SystemManager::OnSceneActivate(Scene& scene) noexcept {
		for (int i = 0; i < m_modules.size(); i++) {

			ISystemModule* module = m_modules[i];
			if (!module || !m_moduleIsInit[i]) continue;

			module->OnSceneActivate(scene);

		}
	}

	void SystemManager::OnSceneDeactivate(Scene& scene) noexcept{
		for (int i = 0; i < m_modules.size(); i++) {

			ISystemModule* module = m_modules[i];
			if (!module || !m_moduleIsInit[i]) continue;

			module->OnSceneDeactivate(scene);

		}
	}

	void SystemManager::OnSceneCreate(Scene& scene) noexcept{
		for (int i = 0; i < m_modules.size(); i++) {

			ISystemModule* module = m_modules[i];
			if (!module || !m_moduleIsInit[i]) continue;

			module->OnSceneCreate(scene);

		}
	}

	void SystemManager::OnSceneDestroy(Scene& scene) noexcept{
		for (int i = 0; i < m_modules.size(); i++) {

			ISystemModule* module = m_modules[i];
			if (!module || !m_moduleIsInit[i]) continue;

			module->OnSceneDestroy(scene);

		}
	}


	

}
