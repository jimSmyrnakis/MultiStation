#pragma once
#include <vector>
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <stdint.h>
#include <stddef.h>
#include "../../Globals/Globals.hpp"
#include <Platform.hpp>
#include "IComponentArray.hpp"
#include <span>
namespace MultiStation{

	

	/**
	 * @author Dimitris Smyrnakis
	 * @class ComponentArray 
	 * @tparam T The component type
	 * @brief This class manages a dynamic array of components of type T, allowing for efficient
	 * addition, removal, and retrieval of components associated with entities. 
	 * @note The address of each component may or may not change when components are added or removed, 
	 * so users should not rely on component addresses remaining stable. 
	 * Instead, they should use the provided methods to access components based on their associated entities.
	 */
	template<class T>
	class ComponentArray : IComponentArray{


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
		 * @note If the entity already has a component, the method returns nullptr and does not add a new component.
		 * Use ReplaceComponent to update existing components.
		 */
		template<typename... Args>
		T* AddComponent(EntID entity , Args&&... args);

		/**
		 * @brief Replaces the component for the specified entity if it exists.
		 * @param[in] entity The entity whose component is to be replaced.
		 * @param[in] args The arguments to construct the new component.
		 * @returns A pointer to the replaced component, or nullptr if the entity does not have one.
		 */
		template<typename... Args>
		T* ReplaceComponent(EntID entity, Args&&... args);

		/**
		 * @brief Removes the component associated with the specified entity.
		 * @param[in] entity The entity whose component is to be removed.
		 * @details If the entity does not have a component, the method does nothing.
		 * If the component is found, it is removed efficiently by swapping it with the last component
		 * costing O(1) time complexity.
		 */
		void RemoveComponent(EntID entity);

		/**
		 * @brief Gets the component associated with the specified entity.
		 * @param[in] entity The entity whose component is to be retrieved.
		 * @return A pointer to the component if found, or nullptr if the entity does not have one.
		 */
		T* GetComponent(EntID entity);

		/**
		 * @brief Gets the component associated with the specified entity.
		 * @param[in] entity The entity whose component is to be retrieved.
		 * @return A pointer to the component if found, or nullptr if the entity does not have one.
		 */
		const T* GetComponent(EntID entity) const;

		/**
		 * @brief Gets the entity associated with the specified component.
		 * @param[in] component The component whose entity is to be retrieved.
		 * @param[out] entity The entity associated with the component.
		 * @returns True if the component is found and the entity is set, false otherwise.
		 * @warning Do not use this method frequently as it has O(n) time complexity.
		 */
		bool GetEntity(T& component, EntID& entity) const;
		

		/**
		 * @brief Gets a const reference to the vector of all components.
		 * @returns A reference to the vector containing all components.
		 */
		std::span<T> GetComponents(void) ;

		/**
		 * @brief Gets a const reference to the vector of all components.
		 * @returns A reference to the vector containing all components.
		 */
		std::span<const T> GetComponents(void) const;

		/**
		 * @brief Checks if the specified entity has an associated component.
		 * @param[in] entity The entity to check.
		 * @returns true if the entity has a component, false otherwise.
		 */
		bool HasEntity(EntID entity) const;

		/**
		 * @brief Returns the unique id of the component type
		 * 
		 */
		CompID GetID(void) const;


		/**
		 * @brief Removes the component associated with the entity, if exist's .
		 * It is a override virtual method from the IComponent class so
		 * the registry can remove all components of a entity when the entity is destroyed
		 * without the need of the templated type .
		 * 
		 * @param[in] entity The entity id to remove the component for.
		 * 
		 * @returns true if succeded , false otherwise . 
		 */
		void RemoveComponentV(EntID entity) override;

		/**
		 * @brief Adds the component if not exist's to the entity 
		 * 
		 * @param entity The entity id
		 * @return pointer to the new component if succeded nullptr otherwise .
		 */
		void* AddComponentV(EntID entity) override;

		/**
		 * @brief Replaces the component if not exist's to the entity 
		 *
		 * @param entity The entity id
		 * @return pointer to the replaced component if succeded nullptr otherwise .
		 */
		void* ReplaceComponentV(EntID entity) override;


		/**
		 * @brief Method of checking if the entity has a component
		 *
		 * @param entity The entity id
		 * @return true if has a component , false otherwise .
		 */
		bool HasComponentV(EntID entity) override;


		/**
		 * @brief Method for getting component of a entity or nullptr if not has one .
		 *
		 * @param entity Entity id.
		 *
		 */
		void* GetComponentV(EntID entity) override;

		/**
		 * @brief Method for getting all components of this type .
		 *
		 * @param entity Entity id.
		 *
		 */
		void* GetComponentsV(void) override;

		/**
		 * @brief Method that returns the size of each component in bytes
		 */
		virtual size_t SizeOfComponent(void) const ;

		/**
		 * @brief Method that returns the number of components .
		 */
		virtual size_t Count(void) const ;


	private:
		std::vector<T> m_components; // A cached component dynamic list
		std::vector<EntID> m_indexToEntity; // component index mapped to entity value
		std::unordered_map<EntID, size_t> m_entityToIndex; // Á
		


		
	};

	



	// OBJECT CREATION , MOVE and DESTROY METHODS
	template<typename T>
	void ComponentArray<T>::RemoveComponentV(EntID entity) {
		if (m_entityToIndex.count(entity))
			this->RemoveComponent(entity);

	}

	template<typename T>
	void* ComponentArray<T>::AddComponentV(EntID entity) {
		return (void*)this->AddComponent(entity);
	}

	template<typename T>
	void* ComponentArray<T>::ReplaceComponentV(EntID entity) {
		return (void*)this->ReplaceComponent(entity);
	}

	template<typename T>
	bool ComponentArray<T>::HasComponentV(EntID entity) {
		return (void*)this->HasEntity(entity);
	}

	template<typename T>
	void* ComponentArray<T>::GetComponentV(EntID entity) {
		return (void*)this->GetComponent(entity);
	}

	template<typename T>
	void* ComponentArray<T>::GetComponentsV(void) {
		std::span<T> sp  = this->GetComponents();
		return (void*)sp.data();
	}

	template<typename T>
	size_t ComponentArray<T>::SizeOfComponent(void) const {
		return sizeof(T);
	}

	template<typename T>
	size_t ComponentArray<T>::Count(void) const {
		return m_components.size();
	}



	template<typename T>
	ComponentArray<T>::ComponentArray(void ) noexcept {
		m_components.reserve(100);
		m_indexToEntity.reserve(100);
		m_entityToIndex.reserve(100);
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
	}

	template<typename T>
	void ComponentArray<T>::Move(ComponentArray* dest, ComponentArray* src) {
		if (dest == src) return;

		dest->m_components = std::move(src->m_components);
		dest->m_entityToIndex = std::move(src->m_entityToIndex);
		dest->m_indexToEntity = std::move(src->m_indexToEntity);
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

	

















	// CREATING , REMOVING  Component's 
	


	template<typename T>
	template<typename... Args>
	T* ComponentArray<T>::AddComponent(EntID entity, Args&&... args)
	{

		// already exists -> update
		if (m_entityToIndex.count(entity))
		{
			MS_WARN("Entity already has a component. Use ReplaceComponent to update it.");
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
	T* ComponentArray<T>::ReplaceComponent(EntID entity, Args&&... args)
	{
		// already exists -> update
		if (m_entityToIndex.count(entity))
		{
			size_t idx = m_entityToIndex[entity];
			m_components[idx] = std::move(T(std::forward<Args>(args)...));
			return &m_components[idx];
		}

		MS_WARN( "Entity does not have a component to replace. Use AddComponent to add it first.");
		return nullptr;
	}



	template<typename T>
	void ComponentArray<T>::RemoveComponent(EntID entity) {
		if (m_components.empty()) {
			MS_WARN("No components to remove.");
			return;
		}

		if (!m_entityToIndex.count(entity)) { // not found entity
			MS_WARN("Entity does not exist's.");
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
	T* ComponentArray<T>::GetComponent(EntID entity) {
		auto it = m_entityToIndex.find(entity);
		if (it == m_entityToIndex.end()) return nullptr;
		return &m_components[it->second];
	}

	template<typename T>
	const T* ComponentArray<T>::GetComponent(EntID entity) const {
		auto it = m_entityToIndex.find(entity);
		if (it == m_entityToIndex.end()) return nullptr;
		return &m_components[it->second];
	}
	
	template<typename T>
	bool ComponentArray<T>::GetEntity(T& component, EntID& entity) const{
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
	std::span<T> ComponentArray<T>::GetComponents(void)  {
		return m_components;
	}

	template<typename T>
	std::span<const T> ComponentArray<T>::GetComponents(void) const {
		return m_components;
	}

	
	template<typename T>
	bool ComponentArray<T>::HasEntity(EntID entity) const {
		return m_entityToIndex.count(entity) != 0;
	}

	template<typename T>
	CompID ComponentArray<T>::GetID(void) const {
		return GetComponentID<T>();
	}



	



}
