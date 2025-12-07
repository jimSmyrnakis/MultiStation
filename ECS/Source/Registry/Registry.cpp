#include "Registry.hpp"

namespace MultiStation {

	Registry::~Registry(void) noexcept {
		for (auto& it_pair : m_typeComponentMap) {
			delete it_pair.second;
		}
	}

}
