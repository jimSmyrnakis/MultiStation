#pragma once
#include "ComponentArray/ComponentArray.hpp"
#include <Platform.hpp>
#include "../EntityManager.hpp"
namespace MultiStation {
	
	/**
	 * @author Dimitris Smyrnakis
	 * @class Registry
	 * @brief The Registry class manages the registration and retrieval of component with different type
	 * , also entity creation / remove and components types to entities signatures .
	 * 
	 */
	class Registry {


	public:

		/**
		 * @brief Constructs a new Registry object.
		 * 
		 * 
		 */
		Registry(void) noexcept = default;

		/**
		 * @brief Destroys the Registry object and cleans up all registered component arrays
		 * that have been registered.
		 *
		 */
		~Registry(void) noexcept ;

		/**
		 * @brief Copy Register not allowed.
		 * 
		 */
		Registry(const Registry&) noexcept = delete;

		/**
		 * @brief Move Register not allowed.
		 */
		Registry(Registry&&) noexcept = delete;

		/**
		 * @brief Copy assignment not allowed.
		 */
		Registry& operator=(const Registry&) noexcept = delete;

		/**
		 * @brief Move assignment not allowed.
		 */
		Registry& operator=(Registry&&) noexcept = delete;

	public: // Entity Operations


		


		/**
		 * 
		 * @brief Deletes the entity and all components associated with it.  
		 * @param entity that will be removed from the registry 
		 * @return true if entity found and deleted (+the components) and false
		 * if there is no entity matching this id in the registry .
 		 */
		bool RemoveEntity(EntID entity);

		/**
		 * @brief Creates a new Entity .
		 * 
		 * @return The new entity id 
		 * @note That can be used on the future for components operations or entity operations . 
		 */
		EntID CreateEntity(void);

		/**
		 * @brief Just returns true if entity with this id exists otherwise false
		 * 
		 * @param entity The entity id .
		 */
		bool HasEntity(EntID entity) const;

		/**
		 * @brief Returns all entities ids existing on the registry
		 * 
		 * @note The Returns vector is const meaning you should not change it's contents 
		 * just use them for update ui's etc. 
		 */
		std::span<const EntID> GetAllEntities(void) const;

		/**
		 * @brief Returns all Components IDS as a mask .
		 * @details Basicly a CompID is a one bit set value and so each component id
		 * has different bit set . Like 0x0001 for Transform and 
		 * 0x0002 for Script as an example .
		 * 
		 * @param entity The entity id
		 *
		 */
		CompID GetSignatureOf(EntID entity) const;
		
	public: // Component's Operations

		/**
		 * @brief Creates and assigns to entity a new component of type T
		 * @tparam T The component type/class .
		 * @tparam Args The arguments to create a new one Component of type T.
		 * @return Pointer to the new components if operations success ,
		 * else if there is a component of type T on this entity then returns nullptr 
		 * @note This method doesn't replace a existing component , if you want to replace it 
		 * use Replace component method. Fethermore to add a component of type T first it must be 
		 * register on the registry .
		 */
		template<typename T, typename... Args>
		T* AddComponent(EntID entity, Args&&... args);

		/**
		 * @brief Replaces a already existing component with a new one .
		 * @tparam T The component type/class .
		 * @tparam Args The arguments to create a new one Component of type T.
		 * @returns The pointer to the new one component inside the registry or 
		 * nullptr if there wasn't any component of this type before for this entity .
		 * @note To create a new one component use AddComponent instead 
		 * . Fethermore to add a component of type T first it must be 
		 * register on the registry .
		 */
		template<typename T, typename... Args>
		T* ReplaceComponent(EntID entity, Args&&... args);

		/**
		 * @brief Removes an existing component from the entity .
		 * @tparam T The component type/class .
		 * @return true if there is a component in this entity of type T to remove
		 * otherwise false. Fethermore if there is no register of this type it will return false
		 * too.
		 */
		template<typename T>
		bool RemoveComponent(EntID entity);

		/**
		 * @brief Returns true if these entity has a component of type T assign ,
		 * otherwise false .
		 * @tparam T The component type/class .
		 */
		template<typename T>
		bool HasComponent(EntID entity) const;

		/**
		 * @brief Returns a span of type T with all existing components of this type .
		 * @tparam T The component type/class .
		 */
		template<typename T>
		std::span<T> GetComponents(void) ;

		/**
		 * @brief Returns a span of type T with all existing components of this type .
		 * @tparam T The component type/class .
		 */
		template<typename T>
		std::span<const T> GetComponents(void) const;

		/**
		 * @brief Returns the component of type T of the entity .
		 * @tparam T The component type/class .
		 * @return The component if is register and entity has one , otherwise nullptr .
		 */
		template<typename T>
		T* GetComponent(EntID entity);

		/**
		 * @brief Returns the component of type T of the entity as const.
		 * @tparam T The component type/class .
		 * @return The component if is register and entity has one , otherwise nullptr .
		 */
		template<typename T>
		const T* GetComponent(EntID entity) const;

		// 
		/**
		 * @brief Helper function for getting the component of a entity without templates .
		 * 
		 * \param component The component id
		 * \param entity The entity id
		 * \return Returns the entity component via the void pointer or nullptr if not exist's .
		 */
		void* GetComponent(CompID component ,EntID entity);

		/**
		 * @brief Helper function for getting the components .
		 *
		 * \param component The component id
		 * \param entity The entity id
		 * \return Returns an array of components via the void pointer .
		 */
		void* GetComponents(CompID component);

		/**
		 * @brief Helper function for checking if this component is register
		 * without templates .
		 * \param component The component ID
		 * \return Returns true if yes false otherwise .
		 */
		bool HasRegister(CompID component) ;

		/**
		 * @brief Helper funtion for retreiving the component size in bytes
		 * 
		 * \param component The Component id
		 * \return Its size and is different from zero if valid otherwise 0 and means 
		 * component is not register .
		 */
		size_t SizeOfComponent(CompID component) ;

		/**
		 * @brief Helper function for retreiving the total components of this 
		 * 
		 * \param component
		 * \return 
		 */
		size_t Count(CompID component) ;

		
	public: // Registry operations

		/**
		 * @brief Registers a new ComponentArray for the specified component type T
		 * if this Component doesn't exist or otherwise does nothing .
		 * @tparam T The type of the component array to register.
		 * @returns ID of the registered component type
		 */
		template<typename T>
		CompID RegisterComponent(void);


		/**
		 * @brief Checks if a ComponentArray for the specified component type T is registered .
		 * @tparam T The type of the component array to check.
		 * @returns true if registered, false otherwise.
		 */
		template<typename T>
		bool HasRegisterComponent(void) const;

		/**
		 * @brief Unregisters the ComponentArray for the specified component type T if is registered,
		 * otherwise does nothing.
		 * @tparam T The type of the component array to unregister.
		 * @returns ID of the unregistered component type
		 */
		template<typename T>
		CompID UnregisterComponent(void);

	private:
		std::unordered_map<CompID, IComponentArray*> m_typeComponentMap;
		EntityManager m_entityManager;



	};

	

	

	// entity Operations
	
	template<typename T>
	bool Registry::HasComponent(EntID entity) const {
		return m_entityManager.HasEntityComponent<T>(entity);
	}






	// Components Operations

	
	template<typename T, typename... Args>
	T* Registry::AddComponent(EntID entity, Args&&... args) {
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

		m_entityManager.AssingToEntity<T>(entity);

		return result;
	}

	
	template<typename T, typename... Args>
	T* Registry::ReplaceComponent(EntID entity, Args&&... args) {
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
	bool Registry::RemoveComponent(EntID entity) {
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
		m_entityManager.RemoveComponentOf<T>(entity);
		bool result = components->RemoveComponent(entity);
		

		

		return result;
	}

	
	template<typename T>
	std::span<T> Registry::GetComponents(void) {
		if (!HasRegisterComponent<T>())
		{
			return {};
		}

		ComponentArray<T>* components = (ComponentArray<T>*)m_typeComponentMap[GetComponentID<T>()];

		return components->GetComponents();
	}

	
	template<typename T>
	std::span<const T> Registry::GetComponents(void) const {
		if (!HasRegisterComponent<T>())
		{
			return {};
		}

		ComponentArray<T>* components = (ComponentArray<T>*)m_typeComponentMap[GetComponentID<T>()];

		return components->GetComponents<T>();
	}

	
	template<typename T>
	T* Registry::GetComponent(EntID entity) {
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

		return components->GetComponent<T>(entity);
	}

	
	template<typename T>
	const T* Registry::GetComponent(EntID entity) const {
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
	CompID Registry::RegisterComponent(void) {
		CompID id = GetComponentID<T>();
		if (HasRegisterComponent<T>()) {
			MS_WARN("Component type already registered in the registry.");
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
	bool Registry::HasRegisterComponent(void) const {
		CompID id = GetComponentID<T>();
		return m_typeComponentMap.count(id) != 0;
	}


	template<typename T>
	CompID Registry::UnregisterComponent(void) {
		if (!HasRegisterComponent<T>()) {
			MS_WARN("Component type not registered in the registry.");

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
