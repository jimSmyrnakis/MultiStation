#pragma once

#include "../Registry/Registry.hpp"
#include <Platform.hpp>
#include <Utilities.hpp>
#include <functional>
#include <mutex>
/**
 * @file ECSManager.hpp
 * @brief There is the ECSManager class that handles entities and components
 * @author Dimitris Smyrnakis
 */
namespace MultiStation {

	class SystemManager;

	/**
	 * @class ECSManager
	 * @brief The ECSManager class is responsible for managing entities and component's .
	 * Components / Entities that Created/Removed/Replaced are not happen directly but they 
	 * requested to be happen in some other safe moment from the System Manager Side
	 * , this is done to ensure that all operations are not going to create data races 
	 * with the systems that are executed in the same phase .
	 */
	class ECSManager {
	
	public:
		/**
		 * @brief Constructs a new ECS Manager object.
		 * Initializes the ECSManager and sets up necessary data structures for managing entities and components.
		 * 
		 */
		ECSManager(void) noexcept;
		/**
		 * @brief Destroys the ECS Manager object and cleans up resources.
		 */
		~ECSManager(void) noexcept;

		/**
		 * @brief Copy , Move constructors and assignment operators are not allowed for ECSManager, 
		 * as it manages resources and state that should not be duplicated or moved.
		 */
		ECSManager(const ECSManager&) noexcept = delete;
		/**
		 * @brief Copy , Move constructors and assignment operators are not allowed for ECSManager,
		 * as it manages resources and state that should not be duplicated or moved.
		 */
		ECSManager(ECSManager&&) noexcept = delete;
		/**
		 * @brief Copy , Move constructors and assignment operators are not allowed for ECSManager,
		 * as it manages resources and state that should not be duplicated or moved.
		 */
		ECSManager& operator=(const ECSManager&) noexcept = delete;
		/**
		 * @brief Copy , Move constructors and assignment operators are not allowed for ECSManager,
		 * as it manages resources and state that should not be duplicated or moved.
		 */
		ECSManager& operator=(ECSManager&&) noexcept = delete;

	public:// User calls for ecs operations


		// Component Management Insertion , Deletion and Retrieval

		/**
		 * @brief Register a new Component of type T to the ecs . This gives the ability to 
		 * add/remove/replace component's of this type to a entity and other functionalities.
		 * @tparam T The type of the component to register.
		 * @warning The component type T must be derived from IComponent<T> and must not be registered already, otherwise assert's.
		 */
		template<typename T>
		void Register(void);

		/**
		 * @brief Check if a component type T is registered to the ecs or not .
		 * @warning The component type T must be derived from IComponent<T> and must not be registered already, otherwise assert's.
		 */
		template<typename T>
		bool HasRegister(void) const;

		/**
		 * @brief Makes Request for constructing a component of type T with variadic parameters to the entity ,
		 * the operation will happen in some other safe moment from the System Manager side .
		 * @tparam T The type of the component to create and add to ecs.
		 * @tparam Args The types of the arguments to construct the component.
		 * @param[in] entity The entity to which the component will be added.
		 * @param[in] args The arguments to construct the component.
		 * @warning The entity must exist's and the component type T must be registered already .
		 */
		template<typename T, typename... Args>
		void AddComponent(uint32_t entity, Args&&... args);

		/**
		 * @brief Makes Request for removing a component of type T from the entity ,
		 * the operation will happen in some other safe moment from the System Manager . Also
		 * if this component is the last of the entity , then the intity is removed .
		 * @tparam T The type of the component to remove.
		 * @warning The entity must exist's , the component type T must be registered already and entity 
		 * must have type T component . 
		 * */
		template<typename T>
		void RemoveComponent(uint32_t entity);

		/**
		 * @brief Replace the component type T of the entity with a new one constructed 
		 * with the provided arguments .
		 * @tparam T The type of the component to replace.
		 * @tparam Args The types of the arguments to construct the new component.
		 * @param[in] entity The entity whose component is to be replaced.
		 * @param[in] args The arguments to construct the new component.
		 * @warning The entity must exist's , the component type T must be registered already and entity 
		 * must have type T component . 
		 */
		template<typename T, typename... Args>
		void ReplaceComponent(uint32_t entity, Args&&... args);

		/**
		 * @brief Gets a pointer to the component of type T associated with the specified entity.
		 * @param[in] entity The entity whose component is to be retrieved.
		 * @tparam T The type of the component to retrieve.
		 * @warning The entity must exist's , the component type T be registered and the entity must have already one. 
		 * Fethermore the returned pointer is not guaranteed to be valid at the next phase or game loop .
		 * @return A pointer to the component if found, or nullptr if the entity does not have one.
		 * 
		 */
		template<typename T>
		T* GetComponent(uint32_t entity);

		/**
		 * @brief Gets a pointer to the component of type T associated with the specified entity.
		 * @param[in] entity The entity whose component is to be retrieved.
		 * @tparam T The type of the component to retrieve.
		 * @warning The entity must exist's , the component type T be registered and the entity must have already one.
		 * Fethermore the returned pointer is not guaranteed to be valid at the next phase or game loop .
		 * @return A pointer to the component if found, or nullptr if the entity does not have one.
		 */
		template<typename T>
		const T* GetComponent(uint32_t entity) const;

		/**
		 * @brief Gets a const reference to the vector of all components of type T. 
		 * @returns A const reference to the vector of all components of type T.
		 * @tparam T The type of the components to retrieve.
		 * @warning The entity must exist's and the component type T be registered .
		 * Fethermore the vector should not change otherwise the behavior is undefined .
		 */
		template<typename T>
		const std::vector<T>& GetComponents(void) const;
		
		/**
		 * @brief Checks if the specified entity has an associated component of type T.
		 * @param entity The entity to check for the component.
		 * @warning The entity must exist's and the component type T be registered .
		 */
		template<typename T>
		bool HasComponent(uint32_t entity) const;



		// Entity Management
		/**
		 * @brief Makes Request for creating a new entity with the specified ID
		 * @param[in] entity The unique identifier for the new entity to be created.
		 * @warning The entity ID must be unique and not already in use , otherwise warning message
		 * is generated .
		 */
		void CreateEntity(uint32_t entity);

		/**
		 * 
		 * @brief Makes Request for destroying an existing entity with the specified ID. 
		 * Also removes all components associated with this entity.
		 * @param[in] entity The unique identifier of the entity to be destroyed.
		 * @warning The entity must exist, otherwise a warning message is generated. 
		 */
		void DestroyEntity(uint32_t entity);

		/**
		 * @brief Checks if the specified entity exists in the ECSManager.
		 * @param[in] entity The unique identifier of the entity to check for existence.
		 * 
		 */
		bool HasEntity(uint32_t entity) const;

		/**
		 * @brief Gets a const reference to the vector of all entities in the system.
		 * @return A const reference to the vector of all entities in the system.
		 * @warning The vector should not change otherwise the behavior is undefined.
		 */
		const std::vector<uint32_t>& GetEntities(void) const;


	private:
		// Call for execute pending operations in the queue, this is called by the SystemManager after each 
		// system execution to ensure that all operations are executed before the next system execution.
		void PollOperations(void);

	private:
		enum OperationType {
			CREATE_COMPONENT,
			REMOVE_COMPONENT,
			REPLACE_COMPONENT,
			CREATE_ENTITY,
			DESTROY_ENTITY
		};
		struct Operation {
			OperationType type;
			uint32_t entity;
			uint32_t componentTypeID; // only for AddComponent and RemoveComponent
			
			std::function<void(Operation op )> callbackFun; // There is not way of remove/adding component 
			// as is based on its Actual templated type, so we need to store a callback function that will be called when the operation is executed to perform the actual add/remove component operation.

		};
	private:
		Registry* m_registry;
		mutable std::mutex m_registerMutex;
		std::vector<uint32_t> m_entities; // list of all entities in the system
		std::vector<uint32_t> m_entityComponentCounts; // per entity component count
		Queue<Operation> m_operationQueue; // queue to store pending operations
		friend class SystemManager; // allow SystemManager to access private members of ECSManager for efficient system execution
	};

}

#include "ECSTemplateImpl.hpp"
