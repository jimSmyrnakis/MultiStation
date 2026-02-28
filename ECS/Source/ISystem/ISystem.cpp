#include "ISystem.hpp"

namespace MultiStation {
	
	uint32_t ISystem::s_typeID = 0;
	ISystem::ISystem(const std::string name) noexcept {
		m_name = name;
	}

	std::string ISystem::GetName(void) noexcept { return m_name; }

	const std::string ISystem::GetName(void) const noexcept { return m_name; }

	// When the System first attached , executes this
	void ISystem::OnAttach(void) noexcept {};

	// Called Per phase via job system , parallel with other systems in the phase
	void ISystem::OnUpdate(float deltaTime) noexcept {};

	// Called from the main thread at input events
	void ISystem::OnEvent(Event& e) noexcept {};

	// Called from the main thread at imgui render update
	void ISystem::OnImGuiRender(float deltaTime) noexcept {};

	// Called from the main thread at Render update , once for frame
	void ISystem::OnRenderUpdate(float deltaTime) noexcept {};

	void ISystem::OnDetach(void) noexcept {};
}
