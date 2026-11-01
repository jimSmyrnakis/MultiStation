#pragma once
#include "ISystemModule.hpp"
#include <vector>
#include <type_traits>
#include <Platform.hpp>
/**
 * @file SystemManager.hpp
 * @author Dimitris Smyrnakis
 * @brief Module ownership and system lifecycle management.
 * @version 0.1
 */
namespace MultiStation{
	/**
	 * @class SystemManager
	 * @brief Owns one module per C++ type and runs its system lifecycle.
	 *
	 * AddModule() constructs and registers modules without initializing them.
	 * InitModules() initializes pending modules in registration order. Only
	 * initialized modules receive system callbacks and frame updates.
	 * FiniModules() shuts down and deletes all modules in reverse registration
	 * order; the destructor calls it automatically.
	 *
	 * Typical usage: AddModule<T>(), InitModules(), OnUpdate(dt) each frame,
	 * then FiniModules() or destruction. Modules added later remain pending
	 * until the next explicit InitModules() call.
	 *
	 * @warning Not thread-safe. From module/system callbacks, do not add or
	 * remove modules, initialize/finalize modules, or recursively call OnUpdate().
	 */
	class SystemManager {

	public:

		/**
		 * @brief Creates an empty manager using the supplied engine context.
		 * @param[in] context Context passed to module Init() and Fini() callbacks.
		 * @pre context must remain valid until this manager has been destroyed.
		 */
		SystemManager(EngineContext& context) noexcept;

		SystemManager(SystemManager&&) = delete;

		SystemManager(const SystemManager&) = delete;
		
		SystemManager& operator=(const SystemManager&) = delete;

		SystemManager& operator=(SystemManager&&) = delete;

		/**
		 * @brief Shuts down and deletes remaining modules in reverse registration order.
		 *
		 * For each module, calls OnDisable() if the system was last processed as
		 * enabled, then OnDettach(), Fini(context), and deletes the module.
		 * Uninitialized modules are deleted without lifecycle callbacks.
		 */
		~SystemManager(void) noexcept;
		
		/**
		 * @brief Processes enabled-state changes, then updates enabled systems.
		 *
		 * First checks every system's IsEnable() flag and calls OnEnable() or
		 * OnDisable() when it differs from the last processed state. A second pass
		 * calls OnUpdate(dt) for systems processed as enabled. Both passes run
		 * sequentially in registration order.
		 * Uninitialized modules are skipped in both passes.
		 *
		 * @param[in] dt Frame delta time forwarded unchanged to each enabled system.
		 * @note SetEnable() changes are processed during the state-check pass.
		 * Changes made during system OnUpdate() callbacks take effect on the next
		 * manager update, including changes to systems that have not updated yet.
		 * @note No fixed timestep updates are performed.
		 */
		void OnUpdate(float dt)noexcept;


		/**
		 * @brief Constructs and owns a module unless its type is already registered.
		 * @tparam T Concrete module type derived from ISystemModule.
		 * @tparam Args Constructor argument types.
		 * @param[in] args Arguments forwarded to T's constructor.
		 * @return true if the module was added or its type was already registered.
		 * An existing module is left unchanged and receives no callbacks.
		 *
		 * Registration does not call Init() or any system callbacks. Call
		 * InitModules() explicitly before expecting the module to update.
		 *
		 * @warning Do not call from module or system lifecycle/update callbacks.
		 * @note Exceptions escaping construction or allocation terminate the process
		 * because this operation is noexcept.
		 */
		template<typename T , typename... Args>
		bool AddModule(Args&&... args)noexcept;

		/**
		 * @brief Shuts down and deletes the module registered for T.
		 * @tparam T Module type derived from ISystemModule.
		 * @return true if removed; false if absent or T is not a module type.
		 *
		 * Calls OnDisable() if the system was last processed as enabled, followed
		 * by OnDettach(), Fini(context), and module deletion. Any pointer returned
		 * by GetModule<T>() becomes invalid.
		 * If the module has not been initialized, only deletion is performed.
		 *
		 * @warning Do not call from module or system lifecycle/update callbacks.
		 */
		template<typename T>
		bool RemoveModule(void)noexcept;

		/**
		 * @brief Checks whether a module of type T is registered.
		 * @tparam T Module type to look up.
		 * @return true if registered; false if absent or T is not a module type.
		 * @note Registration does not imply initialization.
		 */
		template<typename T>
		bool HasModule(void) const noexcept;

		/**
		 * @brief Returns a borrowed, read-only interface to the module of type T.
		 * @tparam T Module type to look up.
		 * @return The module interface, or nullptr if absent or T is not a module type.
		 * @note Ownership stays with the manager. The pointer is valid until the
		 * module is removed, FiniModules() is called, or the manager is destroyed.
		 * The returned module may still be awaiting initialization.
		 */
		template<typename T>
		const ISystemModule* GetModule(void) const noexcept;

		/**
		 * @brief Initializes all pending modules in registration order.
		 *
		 * Calls module Init(context), then system OnAttach(). After attachment,
		 * records IsEnable() and calls OnEnable() only if that flag is true.
		 * Initially disabled systems receive neither OnEnable() nor OnDisable().
		 * Already initialized modules are skipped, so repeated calls initialize
		 * only modules added since the previous call.
		 *
		 * @pre Each module must provide a valid GetSystem() after Init() returns.
		 * @warning Do not call from module or system callbacks.
		 */
		void InitModules(void) noexcept;

		/**
		 * @brief Shuts down, deletes, and unregisters every module in reverse order.
		 *
		 * For initialized modules, calls OnDisable() if last processed as enabled,
		 * then OnDettach() and Fini(context). Uninitialized modules receive no
		 * callbacks. Every module is deleted and all corresponding metadata erased.
		 * Calling this on an empty manager has no effect. New modules may be added
		 * and initialized afterward; finalized instances are not retained.
		 *
		 * @post The manager is empty; all borrowed module pointers are invalid.
		 * @warning Do not call from module or system callbacks.
		 */
		void FiniModules(void) noexcept;


	public: // Scene Callbacks


		/**
		 * @brief Notifies initialized modules that a scene has become active.
		 * @param[in,out] scene Live scene selected for active use.
		 * @details Dispatches in registration order, including modules whose system
		 * updates are disabled. Does not change the active scene itself.
		 * @pre The caller has completed scene creation/loading and deactivated the
		 * previous active scene, if any.
		 * @warning Call at a safe stage after jobs accessing the scene have finished.
		 * Callbacks must not mutate module registration or trigger nested lifecycle
		 * dispatch. These requirements apply to all scene notification methods.
		 */
		void OnSceneActivate(Scene& scene) noexcept;

		/**
		 * @brief Notifies initialized modules that a scene is leaving active use.
		 * @param[in,out] scene Previously active scene, still alive during dispatch.
		 * @details Dispatches in registration order regardless of system enabled
		 * state. The scene may remain loaded and be activated again later.
		 * @pre The caller provides the safe-stage guarantees of OnSceneActivate().
		 */
		void OnSceneDeactivate(Scene& scene) noexcept;

		/**
		 * @brief Notifies initialized modules that a scene is ready for setup.
		 * @param[in,out] scene Newly created or fully deserialized scene.
		 * @details Dispatches in registration order regardless of system enabled
		 * state. Does not activate the scene. Modules initialized after this call
		 * do not automatically receive this notification for existing scenes.
		 * @pre The scene data is ready and the caller provides the safe-stage
		 * guarantees of OnSceneActivate().
		 */
		void OnSceneCreate(Scene& scene) noexcept;

		/**
		 * @brief Notifies initialized modules before a scene is destroyed.
		 * @param[in,out] scene Scene that remains valid until all callbacks return.
		 * @details Dispatches in registration order regardless of system enabled
		 * state. Modules can release scene resources and references here. This
		 * method neither deletes the scene nor saves it to persistent storage.
		 * @pre An active scene has first received OnSceneDeactivate(). The caller
		 * provides the safe-stage guarantees of OnSceneActivate().
		 */
		void OnSceneDestroy(Scene& scene) noexcept;

		

	private:
		/** @brief Owned modules in registration order; all metadata uses the same index. */
		std::vector<ISystemModule*> m_modules;
		/** @brief Last processed enabled state; meaningful only after initialization. */
		std::vector<bool> m_moduleSystemsState;
		/** @brief Whether each module has entered its initialized lifecycle. */
		std::vector<bool> m_moduleIsInit;
		/** @brief Type identifiers aligned with m_modules; enforces one instance per type. */
		std::vector<ModuleID> m_modulesIds;
		/** @brief Borrowed engine context; must outlive the manager. */
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
		m_moduleIsInit.push_back(false);
		
		
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
		
		if (m_moduleIsInit[index]) {
			if (state) {
				imodule->GetSystem().OnDisable();
			}


			// OnDettached callback on system
			imodule->GetSystem().OnDettach();

			// Finilize the module 
			imodule->Fini(m_context);
		}
		

		//remove the module and it's ids
		auto it_imodule = m_modules.begin() + index;
		auto it_state = m_moduleSystemsState.begin() + index;
		auto it_init = m_moduleIsInit.begin() + index;
		
		m_modules.erase(it_imodule);
		m_modulesIds.erase(it);
		m_moduleSystemsState.erase(it_state);
		m_moduleIsInit.erase(it_init);

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
