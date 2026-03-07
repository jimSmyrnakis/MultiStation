#include "IMSSystem.hpp"

namespace MultiStation {
	
	uint32_t IMSSystem::s_typeID = 0;
	IMSSystem::IMSSystem(const std::string name) noexcept {
		m_name = name;
	}

	std::string IMSSystem::GetName(void) noexcept { return m_name; }

	const std::string IMSSystem::GetName(void) const noexcept { return m_name; }

	// When the System first attached , executes this
	void IMSSystem::OnAttach(void) noexcept {};

	// Called Per phase via job system , parallel with other systems in the phase
	void IMSSystem::OnUpdate(float deltaTime) noexcept {};

	// Called from the main thread at input events
	void IMSSystem::OnEvent(Event& e) noexcept {};

	// Called from the main thread at imgui render update
	void IMSSystem::OnEditorUIRender(float deltaTime) noexcept {};

	// Called from the main thread at Render update , once for frame
	void IMSSystem::OnRenderUpdate(float deltaTime) noexcept {};

	void IMSSystem::OnDetach(void) noexcept {};
}
