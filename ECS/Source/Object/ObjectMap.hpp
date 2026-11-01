#pragma once
#include "../Globals/Globals.hpp"
#include <unordered_map>
/**
 * @author Dimitris Smyrnakis 
 * @file ObjectMap.hpp
 * @brief Maps ECS component and module type IDs to runtime object type IDs.
 * @version 0.1
 */
namespace MultiStation{

	/**
	 * @class ObjectMap
	 * @brief Catalog of known component and module types for runtime lookup.
	 * @details Stores CompID-to-ObjID and ModuleID-to-ObjID associations. Each
	 * association describes a C++ type, not an object instance. Repeated addition
	 * of the same type preserves the same association.
	 *
	 * Entries remain until this map is destroyed; removing a module or
	 * unregistering a component elsewhere does not remove its type association.
	 * A successful lookup therefore does not imply that an instance exists or
	 * that the component/module is currently registered with its manager.
	 *
	 * @note IDs are runtime-only and must not be used as persistent identifiers
	 * in saved data. Persistent identity is handled separately, using UUIDs.
	 * @warning Not thread-safe. Serialize additions and lookups when modifying
	 * the map; it does not provide synchronization.
	 */
	class ObjectMap {

	public:

		/**
		 * @brief Records the runtime object type ID associated with component T.
		 * @tparam T Component type whose runtime IDs are associated.
		 * @return GetObjectID<T>(), also stored under GetComponentID<T>().
		 * @note Does not register component storage or create a component instance.
		 */
		template<typename T>
		ObjID AddComponent(void);

		/**
		 * @brief Records the runtime object type ID associated with module T.
		 * @tparam T Module type whose runtime IDs are associated.
		 * @return GetObjectID<T>(), also stored under GetModuleID<T>().
		 * @note Does not construct, initialize, or register a module with a manager.
		 */
		template<typename T>
		ObjID AddModule(void);

		/**
		 * @brief Looks up the runtime object type ID for a known component type.
		 * @param[in] component_id Runtime component type ID to find.
		 * @param[out] object_id Associated object type ID; unchanged if not found.
		 * @return true if an association exists; false otherwise.
		 * @note Does not insert an entry or check component registration elsewhere.
		 */
		bool GetComponentObjectMap(CompID component_id , ObjID& object_id);

		/**
		 * @brief Looks up the runtime object type ID for a known module type.
		 * @param[in] module_id Runtime module type ID to find.
		 * @param[out] object_id Associated object type ID; unchanged if not found.
		 * @return true if an association exists; false otherwise.
		 * @note Does not insert an entry or check whether a module instance exists.
		 */
		bool GetModuleObjectMap(ModuleID module_id, ObjID& object_id);

	private:

		/** @brief Known component type IDs mapped to their runtime object type IDs. */
		std::unordered_map<CompID, ObjID> m_comps;
		/** @brief Known module type IDs mapped to their runtime object type IDs. */
		std::unordered_map<ModuleID, ObjID> m_mdls;

	};

	template<typename T>
	ObjID ObjectMap::AddComponent(void) {
		return m_comps[GetComponentID<T>()] = GetObjectID<T>();
	}

	template<typename T>
	ObjID ObjectMap::AddModule(void) {
		return m_mdls[GetModuleID<T>()] = GetObjectID<T>();
	}

}
