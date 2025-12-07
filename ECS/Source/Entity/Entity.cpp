#include "Entity.hpp"

namespace MultiStation {

	
	Entity::Entity(uint32_t id, std::shared_ptr<Registry> registry) noexcept {
		m_id = id;
		m_registry = registry;
		// if registry is nullptr , then this entity is invalid
		// TODO : fatal error 
	}

	bool Entity::isValid(void) const noexcept {
		return (m_registry != nullptr && m_id != 0);
	}

	Entity::Entity(Entity&& move) noexcept {
		m_id = move.m_id;
		m_registry = std::move(move.m_registry);
		move.m_id = 0;
		move.m_registry = nullptr;
		
	}

	Entity& Entity::operator=(Entity&& move) noexcept {
		if (this != &move) {
			m_id = move.m_id;
			m_registry = std::move(move.m_registry);
			move.m_id = 0;
			move.m_registry = nullptr;
		}
		return *this;
	}

	uint32_t Entity::GetID(void) const noexcept {
		return m_id;
	}




}
