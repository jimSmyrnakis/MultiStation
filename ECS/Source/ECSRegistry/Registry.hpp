#pragma once
#include "ComponentArray/ComponentArray.hpp"
#include <Platform.hpp>
#include "EntityManager.hpp"
#include <stdarg.h>
/**
 * @author Dimitris Smyrnakis
 * @file Registry.hpp
 * @brief This is where the ecs registry class is , this class is responsible 
 * for entities and there components .
 */
namespace MultiStation {
	
	/**
	 * @class ECSRegistry
	 * @brief The ECSRegistry class manages the registration and retrieval of component with different types
	 * , also entity creation / remove and components types to entities signatures .
	 * @warning Not thread-safe.
	 * @note Unexpected exceptions terminate the application.
	 * @note Components (template type T) should support assigments and default contructor .
	 * @version 1.0
	 */
	class ECSRegistry {


	public:

		/**
		 * @brief Constructs a new ECSRegistry object.
		 * 
		 * 
		 */
		ECSRegistry(void) noexcept = default;

		/**
		 * @brief Destroys the ECSRegistry object and cleans up all registered component arrays
		 * that have been registered.
		 *
		 */
		~ECSRegistry(void) noexcept ;

		/**
		 * @brief Copy Register not allowed.
		 * 
		 */
		ECSRegistry(const ECSRegistry&) noexcept = delete;

		/**
		 * @brief Move Register not allowed.
		 */
		ECSRegistry(ECSRegistry&&) noexcept = delete;

		/**
		 * @brief Copy assignment not allowed.
		 */
		ECSRegistry& operator=(const ECSRegistry&) noexcept = delete;

		/**
		 * @brief Move assignment not allowed.
		 */
		ECSRegistry& operator=(ECSRegistry&&) noexcept = delete;

	public: // Entity Operations


		


		/**
		 * 
		 * @brief Deletes the entity and all components associated with it.  
		 * @param entity that will be removed from the ECSRegistry 
		 * @return true if entity found and deleted (+the components) and false
		 * if there is no entity matching this id in the ECSRegistry .
 		 */
		bool RemoveEntity(EntID entity) noexcept;

		/**
		 * @brief Creates a new Entity .
		 * 
		 * @return The new entity id or nullEntity if there is no available ids .
		 * 
		 */
		EntID CreateEntity(void) noexcept;

		/**
		 * @returns true if entity with this id exists otherwise false
		 * 
		 * @param entity The entity id .
		 */
		bool HasEntity(EntID entity) const noexcept;

		/**
		 * @brief Returns all entities ids existing on the ECSRegistry
		 * 
		 * @note The Returns vector is const meaning you should not change it's contents 
		 * just use them .
		 * @note The span is a view of the entity array meaning there is no guarantees 
		 * if another entity added/removed that the span will be valid . 
		 */
		std::span<const EntID> GetAllEntities(void) const noexcept;

		/**
		 * @brief Returns all Components IDS of the entity  .
		 * @note The span is a view of the entity components array meaning there is no guarantees 
		 * if entity added/removed component that the span will be valid .
		 * 
		 * @param entity The entity id
		 *
		 */
		std::span<const CompID> GetSignatureOf(EntID entity) const noexcept;
		
	public: // Component's Operations

		/**
		 * @brief Creates and assigns to entity a new component of type T
		 * @tparam T The component type/class .
		 * @tparam Args The arguments to create a new one Component of type T.
		 * @return Pointer to the new components if operations success ,
		 * else if there is a component of type T on this entity then returns nullptr 
		 * @note This method doesn't replace a existing component , if you want to replace it 
		 * use Replace component method. Fethermore to add a component of type T first it must be 
		 * register on the ECSRegistry .
		 */
		template<typename T, typename... Args>
		T* AddComponent(EntID entity, Args&&... args) noexcept;

		/**
		 * @brief Replaces a already existing component with a new one .
		 * @tparam T The component type/class .
		 * @tparam Args The arguments to create a new one Component of type T.
		 * @returns The pointer to the new one component inside the ECSRegistry or 
		 * nullptr if there wasn't any component of this type before for this entity .
		 * @note To create a new one component use AddComponent instead 
		 * . Fethermore to add a component of type T first it must be 
		 * register on the ECSRegistry .
		 */
		template<typename T, typename... Args>
		T* ReplaceComponent(EntID entity, Args&&... args) noexcept;

		/**
		 * @brief Removes an existing component from the entity .
		 * @tparam T The component type/class .
		 * @return true if entity exist's and component is registered , otherwise
		 * false . So even if there is no component of this type it will still return
		 * true . 
		 */
		template<typename T>
		bool RemoveComponent(EntID entity) noexcept;

		/**
		 * @returns true if these entity has a component of type T assign ,
		 * otherwise false .
		 * @tparam T The component type/class .
		 */
		template<typename T>
		bool HasComponent(EntID entity) const noexcept;

		/**
		 * @returns a span of type T with all existing components of this type .
		 * @tparam T The component type/class .
		 * @note The span is just a view , if used at the same time with adding/removing
		 * components then the span will probably be not valid .
		 */
		template<typename T>
		std::span<T> GetComponents(void) noexcept ;

		/**
		 * @returns a span of type T with all existing components of this type .
		 * @tparam T The component type/class .
		 * @note The span is just a view , if used at the same time with adding/removing
		 * components then the span will probably be not valid .
		 */
		template<typename T>
		std::span<const T> GetComponents(void) const noexcept ;

		/**
		 * @brief Returns the component of type T of the entity .
		 * @tparam T The component type/class .
		 * @return The component if is register and entity has one , otherwise nullptr .
		 */
		template<typename T>
		T* GetComponent(EntID entity) noexcept;

		/**
		 * @brief Returns the component of type T of the entity as const.
		 * @tparam T The component type/class .
		 * @return The component if is register and entity has one , otherwise nullptr .
		 */
		template<typename T>
		const T* GetComponent(EntID entity) const noexcept;





	public: // Untemplated component methods
		/**
		 * @brief Getting the component of a entity without templates .
		 * 
		 * @param component The component id
		 * @param entity The entity id
		 * @returns the entity component via the void pointer or nullptr if not exist's .
		 */
		void* GetComponent(CompID component ,EntID entity) noexcept;

		/**
		 * @brief Helper method for getting the components without templates.
		 *
		 * @param component The component id
		 * @param entity The entity id
		 * @returns an array of components via the void pointer .
		 */
		void* GetComponents(CompID component) noexcept;

		/**
		 * @brief Removes the component from the entity . As well
		 * the component is destroyed .
		 * @param entity The entity id
		 * @param component The Component id
		 * @return True if entity exist's , component is registered 
		 * otherwise false .
		 */
		bool RemoveComponent(EntID entity , CompID component) noexcept;

		/**
		 * @brief Creates a new component for the entity if has register .
		 * @param entity The entity id
		 * @param component The component id 
		 * @returns The new component address if component is registered and 
		 * entity hasn't any otherwise nullptr .
		 */
		void* AddComponent(EntID entity , CompID component) noexcept;

		/**
		 * @brief Replace's an already existing component with a new one , only
		 * if the entity has one .
		 * 
		 * @param entity The entity id
		 * @param component The component id
		 * @returns The replaced component address if component is registerd 
		 * and entity has one , otherwise nullptr . 
		 */
		void* ReplaceComponent(EntID entity , CompID component) noexcept ;

		/**
		 * @brief Helper method of checking if the entity has a component
		 * without a template type use .
		 * @param entity The entity id
		 * @return true if has a component , false otherwise .
		 */
		bool HasComponent(EntID entity, CompID component) noexcept;

		/**
		 * @brief Helper funtion for retreiving the component size in bytes
		 * 
		 * @param component The Component id
		 * @return Its size and is different from zero if valid otherwise 0 and means 
		 * component is not register .
		 */
		size_t SizeOfComponent(CompID component) noexcept ;

		/**
		 * @brief Helper function for retreiving the total components of this 
		 * 
		 * @param component The Component id
		 * @returns The total components number available in the span list 
		 * with this component id .
		 */
		size_t Count(CompID component) noexcept ;


		

		
	public: // ECSRegistry Registration methods - Templated

		/**
		 * @brief Registers a new ComponentArray for the specified component type T
		 * if this Component doesn't exist or otherwise does nothing .
		 * @tparam T The type of the component array to register.
		 * @returns ID of the registered component type
		 */
		template<typename T>
		CompID RegisterComponent(void) noexcept ;


		/**
		 * @brief Checks if a ComponentArray for the specified component type T is registered .
		 * @tparam T The type of the component array to check.
		 * @returns true if registered, false otherwise.
		 */
		template<typename T>
		bool HasRegisterComponent(void) const noexcept ;

		

		/**
		 * @brief Unregisters the ComponentArray for the specified component type T if is registered,
		 * otherwise does nothing.
		 * @tparam T The type of the component array to unregister.
		 * @returns ID of the unregistered component type
		 */
		template<typename T>
		CompID UnregisterComponent(void) noexcept ;

	public: // ECSRegistry Registration methods - Non Templated

		/**
		 * @brief Helper function for checking if this component is register
		 * without templates .
		 * @param component The component ID
		 * @returns true if registered false otherwise .
		 */
		bool HasRegister(CompID component) noexcept;

	private:
		std::unordered_map<CompID, IComponentArray*> m_typeComponentMap;
		EntityManager m_entityManager;



	};









































































	

	

	// entity Operations
	
	template<typename T>
	bool ECSRegistry::HasComponent(EntID entity) const noexcept {
		return m_entityManager.HasEntityComponent<T>(entity);
	}






	// Components Operations

	
	template<typename T, typename... Args>
	T* ECSRegistry::AddComponent(EntID entity, Args&&... args) noexcept {
		if (
			!HasRegisterComponent<T>()
			||
			!m_entityManager.HasEntity(entity)
			) {
			return nullptr;
		}

		ComponentArray<T>* components = (ComponentArray<T>*)m_typeComponentMap[GetComponentID<T>()];

		T* result = components->AddComponent(entity , std::forward<Args>(args)...);
		if (result == nullptr) {
			return nullptr;
		}

		m_entityManager.AssignToEntity<T>(entity);

		return result;
	}

	
	template<typename T, typename... Args>
	T* ECSRegistry::ReplaceComponent(EntID entity, Args&&... args) noexcept {
		if (
			!HasRegisterComponent<T>()
			||
			!m_entityManager.HasEntity(entity)
			||
			!m_entityManager.HasEntityComponent<T>(entity)
			) {
			return nullptr;
		}

		ComponentArray<T>* components = (ComponentArray<T>*)m_typeComponentMap[GetComponentID<T>()];

		return components->ReplaceComponent(entity , std::forward<Args>(args)...);
	}

	
	template<typename T>
	bool ECSRegistry::RemoveComponent(EntID entity) noexcept {
		if (
			!HasRegisterComponent<T>()
			||
			!m_entityManager.HasEntity(entity)
			||
			!m_entityManager.HasEntityComponent<T>(entity)
			) {
			return false;
		}

		ComponentArray<T>* components = (ComponentArray<T>*)m_typeComponentMap[GetComponentID<T>()];
		m_entityManager.RemoveComponentOf<T>(entity);
		components->RemoveComponent(entity);
		

		

		return true;
	}

	
	template<typename T>
	std::span<T> ECSRegistry::GetComponents(void) noexcept {
		if (!HasRegisterComponent<T>())
		{
			return {};
		}

		ComponentArray<T>* components = (ComponentArray<T>*)m_typeComponentMap[GetComponentID<T>()];

		return components->GetComponents();
	}

	
	template<typename T>
	std::span<const T> ECSRegistry::GetComponents(void) const noexcept {
		if (!HasRegisterComponent<T>())
		{
			return {};
		}

		ComponentArray<T>* components = (ComponentArray<T>*)m_typeComponentMap[GetComponentID<T>()];

		return components->GetComponents<T>();
	}

	
	template<typename T>
	T* ECSRegistry::GetComponent(EntID entity) noexcept {
		if (
			!HasRegisterComponent<T>()
			||
			!m_entityManager.HasEntity(entity)
			||
			!m_entityManager.HasEntityComponent<T>(entity)
		){
			return nullptr;
		}

		

		ComponentArray<T>* components = (ComponentArray<T>*)m_typeComponentMap[GetComponentID<T>()];

		return components->GetComponent(entity);
	}

	
	template<typename T>
	const T* ECSRegistry::GetComponent(EntID entity) const noexcept {
		if (
			!HasRegisterComponent<T>()
			||
			!m_entityManager.HasEntity(entity)
			||
			!m_entityManager.HasEntityComponent<T>(entity)
			) {
			return nullptr;
		}



		ComponentArray<T>* components = (ComponentArray<T>*)m_typeComponentMap[GetComponentID<T>()];

		return components->GetComponent<T>(entity);
	}











	// Registration operations

	template<typename T>
	CompID ECSRegistry::RegisterComponent(void) noexcept {
		CompID id = GetComponentID<T>();
		if (HasRegisterComponent<T>()) {
			MS_WARN("Component type already registered in the ECSRegistry.");
			return id;
		}
		
		// else register that
		ComponentArray<T>* carr = new(std::nothrow) ComponentArray<T>();
		if (carr == nullptr) {
			MS_WARN( "Failed to allocate memory for ComponentArray.");
			return id;
		}
		m_typeComponentMap[id] = (IComponentArray*)carr;

		return id;
	}

	template<typename T>
	bool ECSRegistry::HasRegisterComponent(void) const noexcept {
		CompID id = GetComponentID<T>();
		return m_typeComponentMap.count(id) != 0;
	}


	template<typename T>
	CompID ECSRegistry::UnregisterComponent(void) noexcept {
		if (!HasRegisterComponent<T>()) {
			MS_WARN("Component type not registered in the ECSRegistry.");

			return MultiStation::BAD_ID;
		}
		// else unregister that
		CompID id = GetComponentID<T>();
		// delete it first
		auto it = m_typeComponentMap.find(id);
		if (it != m_typeComponentMap.end()) { 
			delete it->second; 
			m_typeComponentMap.erase(it); 
		}
		return id;
	}


	

}
