#include "Module.hpp"

namespace MultiStation {

	void Transform3DModule::Init(EngineContext& context) noexcept {
		m_system = new TransformSystem(context);
	}


	void Transform3DModule::Fini(EngineContext& context) noexcept {

	}


	ISystem& Transform3DModule::GetSystem(void) noexcept {
		return *m_system;
	}

}
