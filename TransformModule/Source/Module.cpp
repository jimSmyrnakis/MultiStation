#include "Module.hpp"

namespace MultiStation {

	void Transform3DModule::Init(EngineContext& context) {
		m_system = new TransformSystem(context);
	}


	void Transform3DModule::Fini(EngineContext& context) {

	}


	ISystem& Transform3DModule::GetSystem(void) {
		return *m_system;
	}

}
