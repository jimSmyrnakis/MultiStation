#pragma once
#include "ISystemModule.hpp"
#include <vector>
#include <type_traits>
namespace MultiStation{
	/**
	 * @author Dimitris Smyrnakis
	 * @class SystemManager
	 * @brief Responsible for managing the system modules , create them , remove 
	 * update them per frame (loop) .
	 * @warning Modules should avoid accessing these methods for themselfs .
	 */
	class SystemManager {

	public:

		/**
		 * @brief Initiallize the context internal reference
		 * @param[in] context The context reference
		 */
		SystemManager(EngineContext& context);

		

		/**
		 * @brief For each module system calls OnDettach and on module calls Fini  
		 * then deletes it from memory 
		 * 
		 */
		~SystemManager(void);
		
		/**
		 * @brief Updates each system with the delta time from the last frame .
		 * 
		 * @param dt Delta time (time from the previous frame )
		 */
		void OnUpdate(float dt);

		/**
		 * @brief Calls each system OnEvent method with the given event
		 * 
		 * @param e Event type
		 */
		void OnEvent(Event& e);

		/**
		 * @brief If the type T inherits from the ISystemModule then it creates and adds
		 * the new module . Ofcourse calls Init and the system OnAttach methods 
		 * parsing the context or scene .
		 * @warning Do not use this method inside the module init , Fini or System methods .
		 */
		template<typename T>
		bool AddModule(void);

		/**
		 * @brief If the type T inherits from the ISystemModule then it removes and deletes
		 * the new module . Ofcourse calls system OnDettach  and module Fini methods
		 * parsing the context or scene before it deletes them .
		 * @warning Do not use this method inside the module init , Fini or System methods .
		 */
		template<typename T>
		bool RemoveModule(void);

		/**
		 * @brief If the T inherits from ISystemModule and exist's it returns true otherwise false .
		 */
		template<typename T>
		bool HasModule(void) const;

		/**
		 * @brief If the T inherits from ISystemModule and exist's it returns the
		 * ISystemModule pointer to this module otherwise nullptr .
		 */
		template<typename T>
		const ISystemModule* GetModule(void) const;
		

	private:
		std::vector<ISystemModule*> m_modules;
		std::vector<ModuleID> m_modulesIds;
		EngineContext& m_context;
	};


	template<typename T>
	bool SystemManager::AddModule(void) {
		

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
		T* module = new T();
		if (module == nullptr) {
			return false;
		}
		ISystemModule* imodule = (ISystemModule*)module;

		//insert the module and it's ids
		m_modules.push_back(imodule);
		m_modulesIds.push_back(id);
		

		// Initiallize module
		imodule->Init(m_context);

		// Call system OnAttach
		imodule->GetSystem().OnAttach();
		
		return true;
	}

	template<typename T>
	bool SystemManager::RemoveModule(void) {
		

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
		
		// OnDettached callback on system
		imodule->GetSystem().OnDettach();

		// Finilize the module 
		imodule->Fini(m_context);

		//remove the module and it's ids
		auto it_imodule = m_modules.begin() + index;
		
		m_modules.erase(it_imodule);
		m_modulesIds.erase(it);

		delete imodule;

		return true;
	}

	template<typename T>
	bool SystemManager::HasModule(void) const {
		

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
	const ISystemModule* SystemManager::GetModule(void) const {
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
