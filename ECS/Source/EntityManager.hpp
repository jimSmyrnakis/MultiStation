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
		bool AssignToEntity(EntID entity);

		bool AssignToEntity(EntID entity, CompID component);

		bool HasEntity(EntID entity) const;

		template<typename T>
		bool HasEntityComponent(EntID entity);

		bool HasEntityComponent(EntID entity, CompID component);

		template<typename T>
		bool RemoveComponentOf(EntID entity);

		bool RemoveComponentOf(EntID entity, CompID component);

		std::span<const EntID> GetAllEntities(void) const;

		std::span<const CompID> GetSignatureOf(EntID entity) const;


	private:
		std::vector<EntID> m_entities;
		std::vector<std::vector<CompID>> m_signature;
		EntID m_counter;
	};










	template<typename T>
	bool EntityManager::AssignToEntity(EntID entity) {
		return AssignToEntity(entity, GetComponentID<T>());
	}



	template<typename T>
	bool EntityManager::HasEntityComponent(EntID entity) {
		return HasEntityComponent(entity, GetComponentID<T>());
	}

	template<typename T>
	bool EntityManager::RemoveComponentOf(EntID entity) {
		return RemoveComponentOf(entity, GetComponentID<T>());
	}

}
