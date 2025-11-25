#include "ISystem.hpp"

namespace MultiStation {
	
	uint32_t ISystem::WRITABLE = 0x00000001;
	uint32_t ISystem::SYNCHRONIZED = 0x00000002;

	ISystem::ISystem(uint32_t flags) noexcept {
		m_flags = flags;
	}

	

	uint32_t ISystem::GetFlags(void) const {
		return m_flags;
	}

	void ISystem::SetFlags(uint32_t flags) {
		m_flags = flags;
	}

}
