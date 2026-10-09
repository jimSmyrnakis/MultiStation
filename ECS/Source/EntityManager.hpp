#pragma once
#include "Globals/Globals.hpp"
#include <vector>
#include <span>

namespace MultiStation {


	class EntityManager {
	public:
		EntityManager(void);

		EntID CreateEntity(void);

		bool RemoveEntity(EntID entity);

		template<typename T>
		bool AssingToEntity(EntID entity);

		bool HasEntity(EntID entity) const;

		template<typename T>
		bool HasEntityComponent(EntID entity);

		template<typename T>
		bool RemoveComponentOf(EntID entity);

		std::span<const EntID> GetAllEntities(void) const;

		CompID GetSignatureOf(EntID entity) const;


	private:
		std::vector<EntID> m_entities;
		std::vector<CompID> m_signature;
		EntID m_counter;
	};










	template<typename T>
	bool EntityManager::AssingToEntity(EntID entity) {
		size_t index = 0;
		std::vector<EntID>::iterator it = std::find(m_entities.begin(), m_entities.end(), entity);
		if (it == m_entities.end()) {
			return false;
		}
		index = std::distance(m_entities.begin(), it);

		m_signature[index] |= GetComponentID<T>();
		return true;
		
	}



	template<typename T>
	bool EntityManager::HasEntityComponent(EntID entity) {
		size_t index = 0;
		std::vector<EntID>::iterator it = std::find(m_entities.begin(), m_entities.end(), entity);
		if (it == m_entities.end()) {
			return false;
		}
		index = std::distance(m_entities.begin(), it);

		return m_signature[index] & GetComponentID<T>();
	}

	template<typename T>
	bool EntityManager::RemoveComponentOf(EntID entity) {
		size_t index = 0;
		std::vector<EntID>::iterator it = std::find(m_entities.begin(), m_entities.end(), entity);
		if (it == m_entities.end()) {
			return false;
		}
		index = std::distance(m_entities.begin(), it);

		m_signature[index] &= ~GetComponentID<T>();

		return true;
	}

}
