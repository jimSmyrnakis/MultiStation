#pragma once
#include "ISystemModule.hpp"
#include <vector>
#include <type_traits>
#include <Platform.hpp>
/**
 * @author Dimitris Smyrnakis
 * @file SystemManager.hpp
 * @brief This is where the SystemManager class is that is responsible for handling system modules
 * and their systems .
 * @version 0.1
 */
namespace MultiStation{
	/**
	 * @class SystemManager
	 * @brief Responsible for managing the system modules . Operations like create , remove 
	 * and update for them .
	 * @warning No thread safe  .
	 */
	class SystemManager {

	public:

		/**
		 * @brief Initiallize the System manager 
		 * and parse context to the module callbacks .
		 * @param[in] context The context reference
		 */
		SystemManager(EngineContext& context) noexcept;

		SystemManager(SystemManager&&) = delete;

		SystemManager(const SystemManager&) = delete;
		
		SystemManager& operator=(const SystemManager&) = delete;

		SystemManager& operator=(SystemManager&&) = delete;

		/**
		 * @brief For each module system calls OnDettach and on module calls Fini  
		 * then deletes it from memory 
		 * 
		 */
		~SystemManager(void) noexcept;
		
		/**
		 * @brief Updates each system with the delta time from the last frame and the
		 * fixed delta time system updates methods too .
		 * 
		 * @param dt Delta time (time from the previous frame )
		 */
		void OnUpdate(float dt)noexcept;


		/**
		 * @brief If the type T inherits from the ISystemModule then it creates and adds
		 * the new module . Ofcourse calls Init and the system OnAttach methods 
		 * parsing the context or scene .
		 * @warning Do not use this method inside the module init , Fini or System methods .
		 */
		template<typename T , typename... Args>
		bool AddModule(Args&&... args)noexcept;

		/**
		 * @brief If the type T inherits from the ISystemModule then it removes and deletes
		 * the new module . Ofcourse calls system OnDettach  and module Fini methods
		 * parsing the context or scene before it deletes them .
		 * @warning Do not use this method inside the module init , Fini or System methods .
		 */
		template<typename T>
		bool RemoveModule(void)noexcept;

		/**
		 * @brief If the T inherits from ISystemModule and exist's it returns true otherwise false .
		 */
		template<typename T>
		bool HasModule(void) const noexcept;

		/**
		 * @brief If the T inherits from ISystemModule and exist's it returns the
		 * ISystemModule pointer to this module otherwise nullptr .
		 */
		template<typename T>
		const ISystemModule* GetModule(void) const noexcept;
		

	private:
		std::vector<ISystemModule*> m_modules;
		std::vector<bool> m_moduleSystemsState;
		std::vector<ModuleID> m_modulesIds;
		EngineContext& m_context;
	};


	template<typename T,typename... Args>
	bool SystemManager::AddModule(Args&&... args) noexcept {
		

		if constexpr (std::is_base_of_v<ISystemModule, T> == false) 
		{
			// T does not derive from ISystemModule
			return false;
		}
		ModuleID id = GetModuleID<T>();
		auto it = std::find(m_modulesIds.begin(), m_modulesIds.end(), id);

		if (it != m_modulesIds.end()) {
			// is already there 
			return true;
		}

		// Create module
		T* module = new T(std::forward<Args>(args)...);
		if (module == nullptr) {
			return false;
		}
		ISystemModule* imodule = (ISystemModule*)module;

		//insert the module and it's ids
		m_modules.push_back(imodule);
		m_modulesIds.push_back(id);
		m_moduleSystemsState.push_back(true);

		// Initiallize module
		imodule->Init(m_context);

		// Call system OnAttach
		imodule->GetSystem().OnAttach();

		// Call System OnUpdate
		imodule->GetSystem().OnEnable();
		
		return true;
	}

	template<typename T>
	bool SystemManager::RemoveModule(void) noexcept {
		

		if constexpr (!std::is_base_of_v<ISystemModule, T>)
		{
			// T does not derive from ISystemModule
			return false;
		}

		ModuleID id = GetModuleID<T>();
		auto it = std::find(m_modulesIds.begin(), m_modulesIds.end(), id);

		if (it == m_modulesIds.end()) {
			// if not exists 
			return false;
		}

		// Get module
		size_t index = std::distance(m_modulesIds.begin(), it);
		ISystemModule* imodule = m_modules[index];
		bool state = m_moduleSystemsState[index];
		// OnDisable callback if already is enable 
		if (state) {
			imodule->GetSystem().OnDisable();
		}
			

		// OnDettached callback on system
		imodule->GetSystem().OnDettach();

		// Finilize the module 
		imodule->Fini(m_context);

		//remove the module and it's ids
		auto it_imodule = m_modules.begin() + index;
		auto it_state = m_moduleSystemsState.begin() + index;
		m_modules.erase(it_imodule);
		m_modulesIds.erase(it);
		m_moduleSystemsState.erase(it_state);

		delete imodule;

		return true;
	}

	template<typename T>
	bool SystemManager::HasModule(void) const noexcept {
		

		if constexpr (!std::is_base_of_v<ISystemModule, T>)
		{
			// T does not derive from ISystemModule
			return false;
		}

		ModuleID id = GetModuleID<T>();
		auto it = std::find(m_modulesIds.begin(), m_modulesIds.end(), id);

		if (it == m_modulesIds.end()) {
			// if not exists 
			return false;
		}

		return true;
	}

	template<typename T>
	const ISystemModule* SystemManager::GetModule(void) const  noexcept {
		if constexpr (!std::is_base_of_v<ISystemModule, T>)
		{
			// T does not derive from ISystemModule
			return nullptr;
		}

		ModuleID id = GetModuleID<T>();
		auto it = std::find(m_modulesIds.begin(), m_modulesIds.end(), id);

		if (it == m_modulesIds.end()) {
			// if not exists 
			return nullptr;
		}

		size_t index = std::distance(m_modulesIds.begin(), it);
		ISystemModule* imodule = m_modules[index];

		return imodule;
	}

}
