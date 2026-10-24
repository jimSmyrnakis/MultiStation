#pragma once
#include "SceneGraph.hpp"
#include "../ECSRegistry/Registry.hpp"
#include "../System/SystemManager.hpp"
#include "SceneSerializer.hpp"
#include "../Streams/Streams.hpp"
#include "../Object/ObjectMap.hpp"
#include <memory>

/**
 * @author Dimitris Smyrnakis
 * @file Scene.hpp
 * @brief The most important file for everyone making a system module . The scene contains 
 * all of the components , entities lifetime and entities to entities 
 * and entities to components relation .
 * @version 0.1
 */
namespace MultiStation {
	
	class SceneManager;
	class Scene {

	public:

		Scene(EngineContext& context  , ObjectMap& objMap  ) noexcept ;


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
		std::span<const CompID> GetEntityComponentsIDs(EntID entity) const noexcept;

	public: // Component's Operations

		/**
		 * @brief Constructs a component of type T and assigns it to an entity.
		 * @tparam T The component type.
		 * @tparam Args The constructor argument types.
		 * @param entity The entity to receive the component.
		 * @param args Arguments forwarded to the component constructor.
		 * @return A pointer to the new component, or nullptr if T is not registered,
		 * the entity does not exist, or the entity already has a component of type T.
		 * @note Use ReplaceComponent() to update an existing component.
		 * @warning Adding components of type T may reallocate their storage and invalidate
		 * existing pointers, references, and spans. Removing components or their entities
		 * may invalidate the returned pointer or change which entity it refers to.
		 * Unregistering T or destroying the registry invalidates it.
		 */
		template<typename T, typename... Args>
		T* AddComponent(EntID entity, Args&&... args) noexcept;

		/**
		 * @brief Assigns a newly constructed value to an entity's existing component.
		 * @tparam T The component type.
		 * @tparam Args The constructor argument types.
		 * @param entity The entity whose component is replaced.
		 * @param args Arguments forwarded to construct the replacement value.
		 * @return A pointer to the updated component, or nullptr if T is not registered,
		 * the entity does not exist, or it has no component of type T.
		 * @note Use AddComponent() to create a missing component.
		 * @warning Later additions of type T may invalidate the returned pointer through
		 * reallocation. Removing components or their entities may invalidate it or change
		 * which entity it refers to. Unregistering T or destroying the registry invalidates it.
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
		std::span<T> GetComponents(void) noexcept;

		/**
		 * @returns a span of type T with all existing components of this type .
		 * @tparam T The component type/class .
		 * @note The span is just a view , if used at the same time with adding/removing
		 * components then the span will probably be not valid .
		 */
		template<typename T>
		std::span<const T> GetComponents(void) const noexcept;

		/**
		 * @brief Returns an entity's component of type T.
		 * @tparam T The component type.
		 * @param entity The entity whose component is requested.
		 * @return A pointer to the component, or nullptr if T is not registered,
		 * the entity does not exist, or it has no component of type T.
		 * @warning Adding components of type T may invalidate the pointer through
		 * reallocation. Removing components or their entities may invalidate it or change
		 * which entity it refers to. Unregistering T or destroying the registry invalidates it.
		 */
		template<typename T>
		T* GetComponent(EntID entity) noexcept;

		/**
		 * @brief Returns read-only access to an entity's component of type T.
		 * @tparam T The component type.
		 * @param entity The entity whose component is requested.
		 * @return A const pointer to the component, or nullptr if T is not registered,
		 * the entity does not exist, or it has no component of type T.
		 * @warning Adding components of type T may invalidate the pointer through
		 * reallocation. Removing components or their entities may invalidate it or change
		 * which entity it refers to. Unregistering T or destroying the registry invalidates it.
		 */
		template<typename T>
		const T* GetComponent(EntID entity) const noexcept;


		/**
		 * @brief Returns the stored name of component type T.
		 * @tparam T The component type whose name is requested.
		 * @return A const reference to the registered type's name, or the shared
		 * fallback string "none" if T is not registered.
		 * @note Registered names come from typeid(T).name() and are implementation-defined.
		 * @warning A reference to a registered name becomes invalid when T is unregistered
		 * or this registry is destroyed. Copy the string if it must outlive that storage.
		 * The fallback string has static storage duration and is independent of the registry.
		 */
		template<typename T>
		const std::string& GetName(void) const noexcept;


	public: // Untemplated component methods
		/**
		 * @brief Returns an entity's component using its runtime component type ID.
		 * @param component The component type ID.
		 * @param entity The entity whose component is requested.
		 * @return A pointer to the component, or nullptr if the type is not registered
		 * or no component of that type is stored for the entity.
		 * @warning Adding components of this type may invalidate the pointer through
		 * reallocation. Removing components or their entities may invalidate it or change
		 * which entity it refers to. Unregistering the type or destroying the registry invalidates it.
		 */
		void* GetComponent(CompID component, EntID entity) noexcept;

		/**
		 * @brief Returns the contiguous component storage for a runtime component type ID.
		 * @param component The component type ID.
		 * @return A pointer to the storage, or nullptr if the type is not registered.
		 * For an empty registered array, the pointer may be null; use Count() to determine
		 * the number of components and SizeOfComponent() for their size in bytes.
		 * @warning Adding components of this type may invalidate the pointer through
		 * reallocation. Removing components or their entities changes the stored sequence
		 * and may invalidate pointers to its elements or change their entity associations.
		 * Unregistering the type or destroying the registry invalidates the storage pointer.
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
		bool RemoveComponent(EntID entity, CompID component) noexcept;

		/**
		 * @brief Default-constructs a component and assigns it to an entity.
		 * @param entity The entity to receive the component.
		 * @param component The registered component type ID.
		 * @return A pointer to the new component, or nullptr if the type is not registered,
		 * the entity does not exist, or it already has a component of this type.
		 * @warning Adding components of this type may invalidate existing pointers,
		 * references, and spans through reallocation. Removing components or their entities
		 * may invalidate the returned pointer or change which entity it refers to.
		 * Unregistering the type or destroying the registry invalidates it.
		 */
		void* AddComponent(EntID entity, CompID component) noexcept;

		/**
		 * @brief Assigns a default-constructed value to an entity's existing component.
		 * @param entity The entity whose component is replaced.
		 * @param component The registered component type ID.
		 * @return A pointer to the updated component, or nullptr if the type is not registered,
		 * the entity does not exist, or it has no component of this type.
		 * @warning Later additions of this type may invalidate the returned pointer through
		 * reallocation. Removing components or their entities may invalidate it or change
		 * which entity it refers to. Unregistering the type or destroying the registry invalidates it.
		 */
		void* ReplaceComponent(EntID entity, CompID component) noexcept;

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
		size_t SizeOfComponent(CompID component) noexcept;

		/**
		 * @brief Helper function for retreiving the total components of this
		 *
		 * @param component The Component id
		 * @returns The total components number available in the span list
		 * with this component id .
		 */
		size_t Count(CompID component) noexcept;

		/**
		 * @brief Returns the stored name of a component type using its runtime ID.
		 * @param component The component type ID whose name is requested.
		 * @return A const reference to the registered type's name, or the shared
		 * fallback string "none" if the ID is not registered.
		 * @note Registered names come from typeid(T).name() and are implementation-defined.
		 * @warning A reference to a registered name becomes invalid when that type is
		 * unregistered or this registry is destroyed. Copy the string if it must outlive
		 * that storage. The fallback string has static storage duration and is independent
		 * of the registry.
		 */
		const std::string& GetName(CompID component) const noexcept;



	public: // ECSRegistry Registration methods - Templated

		/**
		 * @brief Registers storage for component type T if it is not already registered.
		 * @tparam T The component type to register.
		 * @return The component type ID on success, including when T is already registered,
		 * or 0 if allocation of the ComponentArray object returns nullptr.
		 * @note Allocation failures inside the ComponentArray constructor or the registry
		 * map, and any other escaping exceptions, terminate the application under the
		 * noexcept policy; they are not reported by returning 0.
		 */
		template<typename T>
		CompID RegisterComponent(void) noexcept;


		/**
		 * @brief Checks if a ComponentArray for the specified component type T is registered .
		 * @tparam T The type of the component array to check.
		 * @returns true if registered, false otherwise.
		 */
		template<typename T>
		bool HasRegisterComponent(void) const noexcept;



		/**
		 * @brief Unregisters component type T and destroys all its stored components.
		 * @tparam T The component type to unregister.
		 * @details Removes T from every entity's signature without removing the entities.
		 * If T is not registered, the registry is left unchanged.
		 * @return The unregistered component type ID, or MultiStation::BAD_ID if T
		 * was not registered.
		 * @note Retained for API symmetry with RegisterComponent(); normal registry use
		 * does not require explicit unregistration. Registry destruction releases all storage.
		 * @warning Invalidates all pointers, references, and spans to components of type T.
		 * Views of affected entity signatures may also be invalidated or have changed contents.
		 */
		template<typename T>
		CompID UnregisterComponent(void) noexcept;

	public: // ECSRegistry Registration methods - Non Templated

		/**
		 * @brief Helper function for checking if this component is register
		 * without templates .
		 * @param component The component ID
		 * @returns true if registered false otherwise .
		 */
		bool HasRegister(CompID component) const noexcept;

	public: // Graph Scene relations

		/**
		 * @brief Moves an entity and its subtree beneath the requested parent.
		 * @param entity Existing non-root entity to reparent.
		 * @param parent Existing parent ID; rootEntity attaches the entity to the root.
		 * @return True if reparented or already attached to that parent. Returns false
		 * for missing IDs, nullEntity, a root entity being moved, self-parenting,
		 * or a relationship that would create a cycle.
		 */
		bool SetParent(EntID entity, EntID parent) noexcept;

		/**
		 * @brief Retrieves the direct parent of an entity.
		 * @param entity Entity whose parent is requested.
		 * @param[out] out Receives the parent's ID on success; unchanged on failure.
		 * @return True on success; false if absent or equal to rootEntity or nullEntity.
		 */
		bool GetParent(EntID entity, EntID& out) const noexcept;

		/**
		 * @brief Returns a read-only view of an entity's direct child IDs.
		 * @param entity Parent ID; rootEntity retrieves the top-level entities.
		 * @return Child IDs in stored order, or an empty span if there are no children,
		 * the entity is absent, or entity equals nullEntity.
		 * @warning The span borrows graph storage. Reacquire it after hierarchy changes;
		 * it must not be used after the graph is destroyed.
		 */
		std::span<const EntID> GetChildren(EntID entity) const noexcept;

	
		
	public: // Object Map

		/**
		 * 
		 * @param component_id The Component id 
		 * @param object_id The object id of this component
		 * @return The Component id as a object  
		 */
		bool GetComponentObjectID(CompID component_id, ObjID& object_id) noexcept;

		
	



		

	private:
		friend class SceneSerializer;
		friend class SceneManager;
		// give in the future access only in friend classes


		

	private:
		ECSRegistry m_ecsRegistry;
		SceneGraph m_sceneGraph;
		EngineContext& m_engineContext;
		ObjectMap& m_objectMap;
	};





















	



































	
	template<typename T, typename... Args>
	T* Scene::AddComponent(EntID entity, Args&&... args) noexcept {
		return m_ecsRegistry.AddComponent<T>(entity, std::forward<Args>(args)...);
	}

	
	template<typename T, typename... Args>
	T* Scene::ReplaceComponent(EntID entity, Args&&... args) noexcept {
		return m_ecsRegistry.ReplaceComponent<T>(entity, std::forward<Args>(args)...);
	}

	
	template<typename T>
	bool Scene::RemoveComponent(EntID entity) noexcept {
		return m_ecsRegistry.RemoveComponent<T>(entity);
	}

	
	template<typename T>
	bool Scene::HasComponent(EntID entity) const noexcept {
		return m_ecsRegistry.HasComponent<T>(entity);
	}

	
	template<typename T>
	std::span<T> Scene::GetComponents(void) noexcept {
		return m_ecsRegistry.GetComponents<T>();
	}

	
	template<typename T>
	std::span<const T> Scene::GetComponents(void) const noexcept {
		return m_ecsRegistry.GetComponents<T>();
	}

	
	template<typename T>
	T* Scene::GetComponent(EntID entity) noexcept {
		return m_ecsRegistry.GetComponent<T>(entity);
	}

	
	template<typename T>
	const T* Scene::GetComponent(EntID entity) const noexcept {
		return m_ecsRegistry.GetComponent<T>(entity);
	}


	// Registry operations

	
	template<typename T>
	CompID Scene::RegisterComponent(void) noexcept {
		m_objectMap.AddComponent<T>();
		return m_ecsRegistry.RegisterComponent<T>();
	}


	
	template<typename T>
	bool Scene::HasRegisterComponent(void) const noexcept {
		return m_ecsRegistry.HasRegisterComponent<T>();
	}

	
	template<typename T>
	CompID Scene::UnregisterComponent(void) noexcept {
		return m_ecsRegistry.UnregisterComponent<T>();
	}




	template<typename T>
	const std::string& Scene::GetName(void) const noexcept {
		return m_ecsRegistry.GetName<T>();
	}


}
