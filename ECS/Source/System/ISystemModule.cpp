#include "ISystemModule.hpp"

namespace MultiStation {

	std::atomic<ModuleID> moduleIdGenerator = 0;

	void ISystemModule::OnSceneCreate(Scene& scene) noexcept {

	}


	void ISystemModule::OnSceneActivate(Scene& scene) noexcept {

	}


	void ISystemModule::OnSceneDeactivate(Scene& scene) noexcept {

	}


	void ISystemModule::OnSceneDestroy(Scene& scene) noexcept {

	}

}
