#include "InspectorRegistry.hpp"

namespace MultiStation {

    

    IInspectable* InspectorRegistry::GetInspectable(ObjID id)  {
		auto it = m_Inspectables.find(id);
		if (it != m_Inspectables.end()) {
			return it->second;
		}
		return nullptr;
    }

	bool InspectorRegistry::HasInspectable(ObjID id)  {
		auto it = m_Inspectables.find(id);
		return it != m_Inspectables.end();
	}

}
