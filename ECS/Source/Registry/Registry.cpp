#include "Registry.hpp"

namespace MultiStation {

	Registry::~Registry(void) noexcept {
		for (auto& it_pair : m_typeComponentMap) {
			delete it_pair.second;
		}
	}

	void Registry::RemoveEntity(uint32_t entity) {
		for (auto& it_pair : m_typeComponentMap) {
			IComponentArray* carr = it_pair.second;
			carr->RemoveEntity(entity);

		}
	}

}
