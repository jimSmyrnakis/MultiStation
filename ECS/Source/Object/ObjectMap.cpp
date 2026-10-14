#include "ObjectMap.hpp"

namespace MultiStation {

	bool ObjectMap::GetComponentObjectMap(
		CompID component_id,
		ObjID& object_id)
	{
		auto it = m_comps.find(component_id);

		if (it == m_comps.end())
			return false;

		object_id = it->second;
		return true;
	}

	bool ObjectMap::GetModuleObjectMap(
		ModuleID module_id,
		ObjID& object_id)
	{
		auto it = m_mdls.find(module_id);

		if (it == m_mdls.end())
			return false;

		object_id = it->second;
		return true;
	}

}
