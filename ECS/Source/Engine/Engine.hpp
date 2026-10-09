#pragma once
#include "../Scene/SceneManager.hpp"

#include <Media.hpp>
#include "../Serialization/Registry/SerializationRegistry.hpp"
#include "../System/SystemManager.hpp"
namespace MultiStation {
	class Engine {

	public:

		Engine(void);

		~Engine(void);

		/**
		 * @brief Updates the engine with a event , this way every system receives them 
		 * @note For moment each system takes every event 
		 * 
		 * @param e The event
		 */
		void OnEvent(Event& e);

		/**
		 * @brief Updates all systems of the modules (all logic)
		 * 
		 * @param dt Delta Time
		 */
		void OnUpdate(float dt);

		// SYSTEMS


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



		// Serialization
		/**
		 * @tparam T Type of ISerialize implementation .
		 * @tparam Component The component that will serialize from
		 * @brief The T should inherit from ISerialize and the Component should
		 * be already register as a component to the scene for the registation
		 * to work (return true) otherwise we take false and no registation happens.
		 */
		template<typename Component, typename T>
		bool SerializerRegisterComponent(void);

		/**
		 * @tparam T Type of ISerialize implementation .
		 * @tparam SysModule The Module that will serialize from
		 * @brief The T should inherit from ISerialize for the registation
		 * to work (return true) otherwise we take false and no registation happens.
		 */
		template<typename SysModule, typename T>
		bool SerializerRegisterSystem(void);

		/**
		 * @tparam T Type of ISerialize implementation .
		 * @tparam Object The Generic Object that will serialize from
		 * @brief The T should inherit from ISerialize  for the registation
		 * to work (return true) otherwise we take false and no registation happens.
		 * Objects are more special support , that can saved to the scene . This means
		 * their changes remains for ever in the scene final file etc. So carefull on that .
		 */
		template<typename Object, typename T>
		bool SerializerRegisterObject(void);


	private:
		SerializationRegistry m_serializationRegistry;
		SceneManager* m_sceneManager;
		SystemManager* m_systemManager;
		EngineContext m_context;
	};

	// SYSTEM


	template<typename T>
	bool Engine::AddModule(void) {
		return m_systemManager->AddModule<T>();
	}


	template<typename T>
	bool Engine::RemoveModule(void) {
		return m_systemManager->RemoveModule<T>();
	}


	template<typename T>
	bool Engine::HasModule(void) const {
		return m_systemManager->HasModule<T>();
	}


	template<typename T>
	const ISystemModule* Engine::GetModule(void) const {
		return m_systemManager->GetModule<T>();
	}

	template<typename Component, typename T>
	bool Engine::SerializerRegisterComponent(void) {

		if constexpr (!std::is_base_of<ISerialize, T>()) {
			return false;
		}
		T* Seriali = new T();
		if (Seriali == nullptr) {
			return false;
		}
		return m_serializationRegistry.RegisterComponent(GetComponentID<Component>(), Seriali);
	}


	template<typename SysModule, typename T>
	bool Engine::SerializerRegisterSystem(void) {
		
		if constexpr (!std::is_base_of<ISerialize, T>()) {
			return false;
		}
		T* Seriali = new T();
		if (Seriali == nullptr) {
			return false;
		}
		return m_serializationRegistry.RegisterSystem(GetModuleID<SysModule>(), Seriali);
	}

	template<typename Object, typename T>
	bool Engine::SerializerRegisterObject(void) {


		if constexpr (!std::is_base_of<ISerialize, T>()) {
			return false;
		}
		T* Seriali = new T();
		if (Seriali == nullptr) {
			return false;
		}
		return m_serializationRegistry.RegisterObject(GetObjectID<Object>(), Seriali);
	}
};
