#pragma once
#include "../Registry/Registry.hpp"

namespace MultiStation {

	/**
	 * @author Dimitris Smyrnakis
	 * @class Entity
	 * @brief A wrapper class that represents an entity in the ECS architecture , 
	 * but is not a crucial part of it . This class provides methods to interact 
	 * with the components of an entity through the Registry.
	 * @warning This class is a simple wrapper and does not manage the lifecycle of entities.
	 * and components. The registry parsed in the constructor must be valid during the lifetime of this object.
	 * 
	 */
	class Entity {
		bool isValid(void) const noexcept;
	public:

		/**
		 * @brief Constructs a new Entity object with the specified ID and registry.
		 * @param[in] id The unique identifier for the entity.
		 * @param[in] registry A shared pointer to the Registry managing the entity's components.
		 * @warning The registry given must not be nullptr . 
		 */
		Entity(uint32_t id, std::shared_ptr<Registry> registry) noexcept;

		/**
		 * @brief Destroys the Entity object.
		 */
		~Entity(void) noexcept = default;
		
		/**
		 * @brief Copy constructor deleted to prevent copying.
		 */
		Entity(const Entity& cpy) noexcept = delete;
		
		/**
		 * @brief Copy assignment operator deleted to prevent copying.
		 */
		Entity& operator=(const Entity& cpy) noexcept = delete;
		
		/**
		 * @brief Move constructor for transferring ownership.
		 * @param[in] move The Entity to move from.
		 */
		Entity(Entity&& move) noexcept;
		
		/**
		 * @brief Move assignment operator for transferring ownership.
		 * @param[in] move The Entity to move from.
		 * @returns A reference to the moved Entity.
		 */
		Entity& operator=(Entity&& move) noexcept;
		

		/**
		 * @brief Gets the unique identifier of the entity.
		 * @returns The unique identifier of the entity.
		 */
		uint32_t GetID(void) const noexcept;
		
		/**
		 * @brief Gets the component of type T associated with the entity.
		 * @tparam T The type of the component to retrieve.
		 * @returns A pointer to the component of type T if it exists, or nullptr if the entity does not have such a component.
		 */
		template<typename T>
		T* GetComponent(void) const;

		/**
		 * @brief Adds a component of type T to the entity.
		 * @tparam Args Variadic template parameters for the component constructor.
		 * @tparam T The type of the component to add.
		 * @param[in] args The arguments to construct the component (Forwarding variadic params).
		 * @returns A pointer to the newly added component of type T or nullptr if the entity already has such a component.
		 */
		template<typename T, typename... Args>
		T* AddComponent(Args&&... args);

		/**
		 * @brief Removes the component of type T from the entity.
		 * @tparam T The type of the component to remove.
		 * @note If the entity does not have a component of type T, the method does nothing.
		 */
		template<typename T>
		void RemoveComponent(void);

		/**
		 * @brief Checks if the entity has a component of type T.
		 * @tparam T The type of the component to check.
		 * @returns true if the entity has a component of type T, false otherwise.
		 */
		template<typename T>
		bool HasComponent(void) const;

	private :
		uint32_t m_id;
		std::shared_ptr<Registry> m_registry;

	};






	
	template<typename T>
	T* Entity::GetComponent(void) const {
		if (!isValid()) {
			// TODO : fatal error 
			return nullptr;
		}
		ComponentArray<T>* compArray = m_registry->GetComponentArray<T>();
		if (compArray == nullptr) {
			// TODO : error to end user
			return nullptr;
		}
		return compArray->GetComponent(m_id);
	}

	template<typename T, typename... Args>
	T* Entity::AddComponent(Args&&... args) {
		if (!isValid()) {
			// TODO : fatal error 
			return nullptr;
		}
		ComponentArray<T>* compArray = m_registry->GetComponentArray<T>();
		if (compArray == nullptr) {
			// TODO : error to end user
			return nullptr;
		}
		return compArray->AddComponent(m_id, std::forward<Args>(args)...);
	}

	template<typename T>
	void Entity::RemoveComponent(void) {
		if (!isValid()) {
			// TODO : fatal error 
			return ;
		}
		ComponentArray<T>* compArray = m_registry->GetComponentArray<T>();
		if (compArray == nullptr) {
			// TODO : error to end user
			return;
		}
		compArray->RemoveComponent(m_id);
	}

	template<typename T>
	bool Entity::HasComponent(void) const {
		if (!isValid()) {
			// TODO : fatal error 
			return false;
		}
		ComponentArray<T>* compArray = m_registry->GetComponentArray<T>();
		if (compArray == nullptr) {
			// TODO : error to end user
			return false;
		}
		return compArray->HasEntity(m_id);
	}




}
