#pragma once
#include "../Globals/Globals.hpp"
#include <unordered_map>
namespace MultiStation{

	class ObjectMap {

	public:

		template<typename T>
		ObjID AddComponent(void);

		template<typename T>
		ObjID AddModule(void);

		bool GetComponentObjectMap(CompID component_id , ObjID& object_id);

		bool GetModuleObjectMap(ModuleID module_id, ObjID& object_id);

	private:

		std::unordered_map<CompID, ObjID> m_comps;
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
