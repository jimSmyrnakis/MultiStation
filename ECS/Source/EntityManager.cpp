#include "EntityManager.hpp"

namespace MultiStation {

	EntityManager::EntityManager(void) {
		m_counter = 0;
	}
	 
	EntID EntityManager::CreateEntity( void ) {
		m_counter++;
		m_entities.push_back(m_counter);
		m_signature.push_back({});
		return m_counter;
	}

	bool EntityManager::RemoveEntity(EntID entity) {
		size_t index = 0;
		std::vector<EntID>::iterator it_entity = std::find(m_entities.begin(), m_entities.end(), entity);
		if (it_entity == m_entities.end()) {
			return false;
		}
		index = std::distance(m_entities.begin(), it_entity);

		std::vector<std::vector<CompID>>::iterator it_signature = m_signature.begin() + index;

		m_entities.erase(it_entity);
		m_signature.erase(it_signature);

		return true;
	}


	bool EntityManager::HasEntity(EntID entity) const {
		std::vector<EntID>::const_iterator it_entity = std::find(m_entities.begin(), m_entities.end(), entity);
		if (it_entity == m_entities.end()) {
			return false;
		}
		return true;
	}

	std::span<const EntID> EntityManager::GetAllEntities(void) const {
		return m_entities;
	}
	
	std::span< const CompID> EntityManager::GetSignatureOf(EntID entity) const {
		size_t index = 0;
		std::vector<EntID>::const_iterator it_entity = std::find(m_entities.begin(), m_entities.end(), entity);
		if (it_entity == m_entities.end()) {
			return {};
		}
		index = std::distance(m_entities.begin(), it_entity);

		const std::vector<std::vector<CompID>>::const_iterator it_signature = m_signature.begin() + index;

		return *it_signature;
	}

	bool EntityManager::AssignToEntity(EntID entity, CompID component) {
		size_t index = 0;
		std::vector<EntID>::iterator it = std::find(m_entities.begin(), m_entities.end(), entity);
		if (it == m_entities.end()) {
			return false;
		}
		index = std::distance(m_entities.begin(), it);
		
		auto it_sig = std::find(m_signature[index].begin(), m_signature[index].end(), component);
		if (it_sig != m_signature[index].end())
			return false;

		m_signature[index].push_back( component);
		return true;
	}

	bool EntityManager::HasEntityComponent(EntID entity, CompID component) {
		size_t index = 0;
		std::vector<EntID>::iterator it = std::find(m_entities.begin(), m_entities.end(), entity);
		if (it == m_entities.end()) {
			return false;
		}
		index = std::distance(m_entities.begin(), it);
		auto it_sig = std::find(m_signature[index].begin(), m_signature[index].end(), component);
		return it_sig != m_signature[index].end();
	}

	bool EntityManager::RemoveComponentOf(EntID entity, CompID component) {
		size_t index = 0;
		std::vector<EntID>::iterator it = std::find(m_entities.begin(), m_entities.end(), entity);
		if (it == m_entities.end()) {
			return false;
		}
		index = std::distance(m_entities.begin(), it);
		auto it_sig = std::find(m_signature[index].begin(), m_signature[index].end(), component);
		if (it_sig == m_signature[index].end())
			return false;

		m_signature[index].erase(it_sig);

		return true;
	}

}
