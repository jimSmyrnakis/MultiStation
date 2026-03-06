#pragma once
#include "ECSManager.hpp"
#include <mutex>

namespace MultiStation {


	template<typename T>
	void ECSManager::Register(void) {
		std::lock_guard<std::mutex> lock(m_registerMutex);
		m_registry->Register<T>();
	}

	template<typename T>
	bool ECSManager::HasRegister(void) const {
		std::lock_guard<std::mutex> lock(m_registerMutex);
		return m_registry->HasRegister<T>();
	}

	template<typename T, typename... Args>
	void ECSManager::AddComponent(uint32_t entity, Args&&... args) {
		Operation operation;
		operation.type = OperationType::CREATE;
		operation.entity = entity;
		operation.componentTypeID = IComponentArray::GetID<T>();
		operation.callbackFun = [this , ... args = std::forward<Args>(args)](Operation op ) {
			uint32_t entity = op.entity;
			if (!HasEntity(entity)) {
				MS_ENGINE_WARN("Entity %d does not exist. Skipping adding component.", entity);
				return;
			}
			if (!HasRegister<T>()) {
				MS_ENGINE_WARN("Component type not registered. Skipping adding component.");
				return;
			}
			
			ComponentArray<T>* carr = m_registry->GetComponentArray<T>();
			
			carr->AddComponent(entity, args...);
			auto it = std::find(m_entities.begin(), m_entities.end(), entity);
			int index = std::distance(m_entities.begin(), it);
			m_entityComponentCounts[index]++;
		};
		m_componentOperationQueue.Push(operation);
	}

	template<typename T>
	void ECSManager::RemoveComponent(uint32_t entity) {
		Operation operation;
		operation.type = OperationType::REMOVE;
		operation.entity = entity;
		operation.componentTypeID = IComponentArray::GetID<T>();
		operation.callbackFun = [this, entity](Operation op) {
			if (!HasEntity(entity)) {
				MS_ENGINE_WARN("Entity %d does not exist. Skipping removing component.", entity);
				return;
			}
			if (!HasRegister<T>()) {
				MS_ENGINE_WARN("Component type not registered. Skipping removing component.");
				return;
			}

			ComponentArray<T>* carr = m_registry->GetComponentArray<T>();

			carr->RemoveComponent(entity);
			auto it = std::find(m_entities.begin(), m_entities.end(), entity);
			int index = std::distance(m_entities.begin(), it);
			m_entityComponentCounts[index]--;
			if (m_entityComponentCounts[index] < 0) {
				m_entities.erase(it);
				m_entityComponentCounts.erase(m_entityComponentCounts.begin() + index);
			}
		};

		m_componentOperationQueue.Push(operation);
	}

	template<typename T, typename... Args>
	void ECSManager::ReplaceComponent(uint32_t entity, Args&&... args) {
		Operation operation;
		operation.type = OperationType::REPLACE;
		operation.entity = entity;
		operation.componentTypeID = ComponentArray<T>::GetID();
		operation.callbackFun = [this, ... args = std::forward<Args>(args)](Operation op) {
			uint32_t entity = op.entity;
			if (!HasEntity(entity)) {
				MS_ENGINE_WARN("Entity %d does not exist. Skipping replacing component.", entity);
				return;
			}

			if (!HasRegister<T>()) {
				MS_ENGINE_WARN("Component type not registered. Skipping replacing component.");
				return;
			}

			ComponentArray<T>* carr = m_registry->GetComponentArray<T>();

			if (!carr->HasEntity(entity)) {
				MS_ENGINE_WARN("Entity %d does not have a component to replace. Skipping replacing component.", entity);
				return;
			}

			carr->ReplaceComponent(entity, args...);
			
		};
		m_componentOperationQueue.Push(operation);
	}

	template<typename T>
	T* ECSManager::GetComponent(uint32_t entity) {
		return m_registry->GetComponentArray<T>()->GetComponent(entity);
	}

	template<typename T>
	const T* ECSManager::GetComponent(uint32_t entity) const {
		return m_registry->GetComponentArray<T>()->GetComponent(entity);
	}
	template<typename T>
	std::vector<T>& ECSManager::GetComponents(void) {
		return m_registry->GetComponentArray<T>()->GetComponents();
	}
	template<typename T>
	const std::vector<T>& ECSManager::GetComponents(void) const {
		return m_registry->GetComponentArray<T>()->GetComponents();
	}

	template<typename T>
	bool ECSManager::HasComponent(uint32_t entity) const {
		if (!HasEntity(entity)) {
			//MS_ENGINE_WARN("Entity %d does not exist. Skipping checking component.", entity);
			return false;
		}
		if (!HasRegister<T>()) {
			//MS_ENGINE_WARN("Component type not registered. Skipping checking component.");
			return false;
		}
		return m_registry->GetComponentArray<T>()->HasEntity(entity);
	}

}
