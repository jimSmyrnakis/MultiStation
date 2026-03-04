#include "VersionControl.hpp"

namespace MultiStation {


	// Thread safe
	uint32_t VersionControl::GetVersion(void) const {
		std::shared_lock<std::shared_mutex> lock(m_mutex);
		return m_version;
	}
	// Thread safe
	void VersionControl::IncrementVersion(void) {
		std::unique_lock<std::shared_mutex> lock(m_mutex);
		m_version++;
	}

}
