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
		operation.callbackFun = [this](Operation op) {
			if (std::find(m_entities.begin(), m_entities.end(), op.entity) != m_entities.end()) {
				MS_ENGINE_WARN("Entity %d already exists. Skipping creation.", op.entity);
				return;
			}
			m_entities.push_back(op.entity);
			m_entityComponentCounts.push_back(0); // Initialize component count for the new entity
		};
		m_entitiesOperationQueue.Push(operation);
	}

	void ECSManager::DestroyEntity(uint32_t entity) {
		Operation operation;
		operation.type = OperationType::REMOVE;
		operation.entity = entity;
		operation.callbackFun = [this](Operation op) {
			auto it = std::find(m_entities.begin(), m_entities.end(), op.entity);
			if (it == m_entities.end()) {
				MS_ENGINE_WARN("Entity %d does not exist. Skipping destruction.", op.entity);
				return;
			}
			m_entities.erase(it);
			
			m_registry->RemoveEntity(op.entity);
		};
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

			operation.callbackFun(operation);
		}
		// then to components
		while (m_componentOperationQueue.Size()) {
			Operation operation;
			m_componentOperationQueue.Pop(operation);

			// execute operation

			operation.callbackFun(operation);
		}
	}



}
