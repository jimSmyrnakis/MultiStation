#pragma once
#include <vector>
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <stdint.h>
#include <stddef.h>
#include "IComponentArray.hpp"
#include "../IComponent/IComponent.hpp"

namespace MultiStation{

	enum class ComponentStatus {
		NONE,
		MODIFIED,
		CREATED,
		DESTROYED
	};

	/**
	 * @author Dimitris Smyrnakis
	 * @class ComponentArray 
	 * @tparam T The component type
	 * @brief This class manages a dynamic array of components of type T, allowing for efficient
	 * addition, removal, and retrieval of components associated with entities. Each component is linked
	 * with one entity, and the class ensures that operations are performed in O(1) time complexity where possible.
	 */
	template<class T>
	class ComponentArray : public IComponentArray{

		static_assert(std::is_base_of<IComponent<T>, T>::value,
			"T must inherit from IComponent<T>");

		void ClearVectors(void);
		void Move(ComponentArray* dest, ComponentArray* src);
	public:

		/**
		 * @brief Constructs a new ComponentArray object.
		 */
		ComponentArray(void) noexcept;

		/**
		 * @brief Destroys the ComponentArray object.
		 */
		~ComponentArray(void) noexcept;

		/**
		 * @brief Deleted copy constructor to prevent copying.
		 */
		ComponentArray(const ComponentArray& copy) = delete;

		/**
		 * @brief Deleted copy assignment operator to prevent copying.
		 */
		ComponentArray& operator=(const ComponentArray& copy) = delete;

		/**
		 * @brief Move constructor and move assignment operator for transferring ownership.
		 * @param[in] move The ComponentArray to move from.
		 */
		ComponentArray(ComponentArray&& move) noexcept;

		/**
		 * @brief Move assignment operator for transferring ownership.
		 * @param[in] move The ComponentArray to move from.
		 * @returns A reference to the moved ComponentArray.
		 */
		ComponentArray& operator=(ComponentArray&& move) noexcept;
		

		
		/**
		 * @brief Adds a new component for the specified entity , If the entity does not already have one.
		 * @param[in] entity The entity to associate the component with.
		 * @param[in] args The arguments to construct the component.
		 * @returns A pointer to the newly added component, or nullptr if the entity already has one.
		 * @note If the entity already has a component, the method returns nullptr and does not add a new component. Use ReplaceComponent to update existing components.
		 */
		template<typename... Args>
		T* AddComponent(uint32_t entity , Args&&... args);

		/**
		 * @brief Replaces the component for the specified entity if it exists.
		 * @param[in] entity The entity whose component is to be replaced.
		 * @param[in] args The arguments to construct the new component.
		 * @returns A pointer to the replaced component, or nullptr if the entity does not have one.
		 */
		template<typename... Args>
		T* ReplaceComponent(uint32_t entity, Args&&... args);

		/**
		 * @brief Removes the component associated with the specified entity.
		 * @param[in] entity The entity whose component is to be removed.
		 * @details If the entity does not have a component, the method does nothing.
		 * If the component is found, it is removed efficiently by swapping it with the last component
		 * costing O(1) time complexity.
		 */
		void RemoveComponent(uint32_t entity);

		/**
		 * @brief Gets the component associated with the specified entity.
		 * @param[in] entity The entity whose component is to be retrieved.
		 * @return A pointer to the component if found, or nullptr if the entity does not have one.
		 */
		T* GetComponent(uint32_t entity) const;

		/**
		 * @brief Gets the entity associated with the specified component.
		 * @param[in] component The component whose entity is to be retrieved.
		 * @param[out] entity The entity associated with the component.
		 * @returns True if the component is found and the entity is set, false otherwise.
		 * @warning Do not use this method frequently as it has O(n) time complexity.
		 */
		bool GetEntity(T& component, uint32_t& entity) const;

		

		/**
		 * @brief Gets a const reference to the vector of all components.
		 * @returns A const reference to the vector containing all components.
		 */
		const std::vector<T>& GetComponents(void) const;

		/**
		 * @brief Checks if the specified entity has an associated component.
		 * @param[in] entity The entity to check.
		 * @returns true if the entity has a component, false otherwise.
		 */
		bool HasEntity(uint32_t entity) const;


	private:
		std::vector<T> m_components; // A cached component dynamic list
		std::vector<uint32_t> m_indexToEntity; // component index mapped to entity value
		std::unordered_map<uint32_t, size_t> m_entityToIndex; // 
		// 
		std::unordered_map<uint32_t, ComponentStatus> m_componentsStatus;
		// each component instanced Type ID mapped to a Status variable


		
	};

	



	// OBJECT CREATION , MOVE and DESTROY METHODS

	template<typename T>
	ComponentArray<T>::ComponentArray(void ) noexcept {
		
	};


	template<typename T>
	ComponentArray<T>::~ComponentArray(void) noexcept {
		ClearVectors();
	}


	template<typename T>
	void ComponentArray<T>::ClearVectors(void) {
		m_components.clear();
		m_entityToIndex.clear();
		m_indexToEntity.clear();
		m_componentsStatus.clear();
	}

	template<typename T>
	void ComponentArray<T>::Move(ComponentArray* dest, ComponentArray* src) {
		if (dest == src) return;

		dest->m_components = std::move(src->m_components);
		dest->m_entityToIndex = std::move(src->m_entityToIndex);
		dest->m_indexToEntity = std::move(src->m_indexToEntity);
		dest->m_componentsStatus = std::move(src->m_componentsStatus);
		src->ClearVectors();
	}

	template<typename T>
	ComponentArray<T>::ComponentArray(ComponentArray&& move) noexcept {
		Move(this, &move);
	}

	template<typename T>
	ComponentArray<T>& ComponentArray<T>::operator=(ComponentArray<T>&& move) noexcept {
		if (this != &move)
			Move(this, &move);

		return *this;
	}

	

















	// CREATING , REMOVING  Component's and Entities



	template<typename T>
	template<typename... Args>
	T* ComponentArray<T>::AddComponent(uint32_t entity, Args&&... args)
	{
		// already exists -> update
		if (m_entityToIndex.count(entity))
		{
			// TODO : error message to user
			return nullptr;
		}

		// new component
		size_t newIndex = m_components.size();
		m_components.emplace_back(std::forward<Args>(args)...);

		m_entityToIndex[entity] = newIndex;
		m_indexToEntity.push_back(entity);

		

		return &m_components[newIndex];
	}

	template<typename T>
	template<typename... Args>
	T* ComponentArray<T>::ReplaceComponent(uint32_t entity, Args&&... args)
	{
		// already exists -> update
		if (m_entityToIndex.count(entity))
		{
			size_t idx = m_entityToIndex[entity];
			m_components[idx] = std::move(T(std::forward<Args>(args)...));
			return &m_components[idx];
		}

		// TODO : error to end user
		return nullptr;
	}



	template<typename T>
	void ComponentArray<T>::RemoveComponent(uint32_t entity) {
		if (m_components.empty()) {
			// TODO : error 
			return;
		}

		if (!m_entityToIndex.count(entity)) { // not found entity
			// TODO : error message to user
			return;
		}

		size_t index = m_entityToIndex[entity];
		size_t lastIndex = m_components.size() - 1;


		if (index != lastIndex) {
			m_components[index] = std::move(m_components[lastIndex]);
			uint32_t lastEntity = m_indexToEntity[lastIndex];
			m_entityToIndex[lastEntity] = index;
			m_indexToEntity[index] = lastEntity;
		}
		 

		// pop
		m_components.pop_back();
		m_indexToEntity.pop_back();
		m_entityToIndex.erase(entity); // erase pair entity key, index vaue
		
		
	}











	// BASIC GET methods


	template<typename T>
	T* ComponentArray<T>::GetComponent(uint32_t entity) const{
		
		if (!m_entityToIndex.count(entity)) {
			// TODO : message to the end user
			return nullptr;
		}

		return &m_components[m_entityToIndex[entity]];
	}
	
	template<typename T>
	bool ComponentArray<T>::GetEntity(T& component, uint32_t& entity) const{
		entity = 0xFFFFFFFF;
		for (size_t i = 0; i < m_components.size(); i++) {
			if (&m_components[i] == &component) {
				entity = m_indexToEntity[i];
				return true;
			}
		}

		return false;
	}

	template<typename T>
	const std::vector<T>& ComponentArray<T>::GetComponents(void) const {
		return m_components;
	}
	
	template<typename T>
	bool ComponentArray<T>::HasEntity(uint32_t entity) const {
		return m_entityToIndex.count(entity) != 0;
	}






}
