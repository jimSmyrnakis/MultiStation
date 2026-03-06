#include "ECSManager.hpp"

namespace MultiStation {

	ECSManager::ECSManager(void) noexcept {
		m_registry = new (std::nothrow) Registry();
		if (m_registry == nullptr) {
			MS_ASSERT(false, "Failed to allocate memory for Registry.");
			// Handle allocation failure as needed (e.g., throw an exception, set an error state, etc.)
		}
	}
	
	ECSManager::~ECSManager(void) noexcept {
		if (m_registry) {
			delete m_registry;
			m_registry = nullptr;
		}
	}


	void ECSManager::CreateEntity(uint32_t entity) {
		Operation operation;
		operation.type = OperationType::CREATE;
		operation.entity = entity;
		operation.callbackFun = nullptr;
		m_entitiesOperationQueue.Push(operation);
	}

	void ECSManager::DestroyEntity(uint32_t entity) {
		Operation operation;
		operation.type = OperationType::REMOVE;
		operation.entity = entity;
		operation.callbackFun = nullptr;
		m_entitiesOperationQueue.Push(operation);
	}

	bool ECSManager::HasEntity(uint32_t entity) const {
		return std::find(m_entities.begin(), m_entities.end(), entity) != m_entities.end();
	}

	const std::vector<uint32_t>& ECSManager::GetEntities(void) const {
		return m_entities;
	}

	void ECSManager::PollOperations(void) {
		// first do things to entities
		while (m_entitiesOperationQueue.Size()) {
			Operation operation;
			m_entitiesOperationQueue.Pop(operation);
			
			// execute operation

			switch (operation.type) {
			case OperationType::CREATE:
				DoCreateEntity(operation.entity);
				break;
			case OperationType::REMOVE:
				DoRemoveEntity(operation.entity);
				break;
			default:
				MS_ENGINE_WARN("Invalid operation type for entity operation. Skipping.");
				break;
			}
		}
		// then to components
		while (m_componentOperationQueue.Size()) {
			Operation operation;
			m_componentOperationQueue.Pop(operation);

			// execute operation

			operation.callbackFun(operation);
		}
	}


	void ECSManager::DoRemoveEntity(uint32_t entity) {
		auto it = std::find(m_entities.begin(), m_entities.end(), entity);
		if (it == m_entities.end()) {
			MS_ENGINE_WARN("Entity %d does not exist. Skipping destruction.", entity);
			return;
		}
		m_entities.erase(it);

		m_registry->RemoveEntity(entity);
	}


	void ECSManager::DoCreateEntity(uint32_t entity) {
		if (std::find(m_entities.begin(), m_entities.end(), entity) != m_entities.end()) {
			MS_ENGINE_WARN("Entity %d already exists. Skipping creation.", entity);
			return;
		}
		m_entities.push_back(entity);
		m_entityComponentCounts.push_back(0); // Initialize component count for the new entity
	}



}
