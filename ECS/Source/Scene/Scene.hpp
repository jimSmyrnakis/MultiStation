#pragma once
#include "SceneGraph.hpp"
#include "../Registry/Registry.hpp"
#include "../System/SystemManager.hpp"
#include "SceneSerializer.hpp"
#include "../Streams/Streams.hpp"

#include <memory>

/**
 * @author Dimitris Smyrnakis
 * @file Scene.hpp
 * @brief The most important file for everyone making a system module . The scene contains 
 * all of the components , system modules , serialization , entities lifetime and entities to entities 
 * and entities to components relation .
 */
namespace MultiStation {
	
	class SceneManager;
	class Scene {

	public:

		Scene(EngineContext& context , SerializationRegistry& SerializeRegistry  );


	// ENTITIES

		/**
		 *
		 * @brief Deletes the entity and all components associated with it.
		 * The removed entity's children are reparented to its parent.
		 * @param entity that will be removed from the registry
		 * @return true if entity found and deleted (+the components) and false
		 * if there is no entity matching this id in the registry .
		 */
		bool RemoveEntity(EntID entity );

		/**
		 * @brief Creates a new Entity and adds the entity as child of the root node .
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
		 * @brief Returns all entities ids existing on the Scene or empty if there isn't any .
		 */
		std::span<const EntID> GetAllEntities(void) const;

		/**
		 * @brief Set's a already existing entity with a new existing parent
		 * 
		 * @param entity The entity id
		 * @param parent The entity parent id
		 * @return true if operation is succesfull or false if is not , that can mean
		 * 1. The Entity or the parent may not exist's
		 */
		bool  SetParent(EntID entity, EntID parent);

		/**
		 * @param entity Entity id
		 * @param out The entity parent id
		 * @return True if the operation succeded otherwise false if entity doesnt exist's. 
		 */
		bool GetParent(EntID entity, EntID& out) const;

		/**
		 * @brief Get's all children of this entity if exist's
		 * 
		 * @param entity The entity id
		 * @return A span with all the entities that are childrens of entity or a empty span
		 * if there is no children or no entity with this id exist's 
		 */
		std::span<const EntID> GetChildren(EntID entity) const;

	// Component's Operations

		/**
		 * @brief Creates and assigns to entity a new component of type T
		 * @tparam T The component type/class .
		 * @tparam Args The arguments to create a new one Component of type T.
		 * @return Pointer to the new components if operations success ,
		 * else if there is a component of type T on this entity then returns nullptr
		 * @note This method doesn't replace a existing component , if you want to replace it
		 * use Replace component method. Fethermore to add a component of type T first it must be
		 * registered . 
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
		 * registered .
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
		std::span<T> GetComponents(void);

		/**
		 * @brief Returns a span of type Const T with all existing components of this type .
		 * @tparam T The component type/class .
		 */
		template<typename T>
		std::span<const T> GetComponents(void) const;

		/**
		 * @brief Returns the component of type T of the entity .
		 * @tparam T The component type/class .
		 * @return The component if is register and entity has one otherwise nullptr .
		 */
		template<typename T>
		T* GetComponent(EntID entity);

		/**
		 * @brief Returns the component of type Const T of the entity as const.
		 * @tparam T The component type/class .
		 * @return The component if is register and entity has one , otherwise nullptr .
		 */
		template<typename T>
		const T* GetComponent(EntID entity) const;


	// Registry operations

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
		bool HasRegisteredComponent(void) const;

		/**
		 * @brief Unregisters the ComponentArray for the specified component type T if is registered,
		 * otherwise does nothing.
		 * @tparam T The type of the component array to unregister.
		 * @returns ID of the unregistered component type
		 */
		template<typename T>
		CompID UnregisterComponent(void);

	
		

	



		

	private:
		friend class SceneSerializer;
		friend class SceneManager;
		// give in the future access only in friend classes


		

	private:
		Registry m_registry;
		SceneGraph m_sceneGraph;
		EngineContext& m_engineContext;
		SerializationRegistry& m_serializationRegistry;
		
	};












	/**
	 * @returns The Scene Serialize (ISerialize) Implementation for the scene 
	 */
	SceneSerializer& GetSceneSerializer(void);









	



































	
	template<typename T, typename... Args>
	T* Scene::AddComponent(EntID entity, Args&&... args) {
		return m_registry.AddComponent<T>(entity, std::forward<Args>(args)...);
	}

	
	template<typename T, typename... Args>
	T* Scene::ReplaceComponent(EntID entity, Args&&... args) {
		return m_registry.ReplaceComponent<T>(entity, std::forward<Args>(args)...);
	}

	
	template<typename T>
	bool Scene::RemoveComponent(EntID entity) {
		return m_registry.RemoveComponent<T>(entity);
	}

	
	template<typename T>
	bool Scene::HasComponent(EntID entity) const {
		return m_registry.HasComponent<T>(entity);
	}

	
	template<typename T>
	std::span<T> Scene::GetComponents(void) {
		return m_registry.GetComponents<T>();
	}

	
	template<typename T>
	std::span<const T> Scene::GetComponents(void) const {
		return m_registry.GetComponents<T>();
	}

	
	template<typename T>
	T* Scene::GetComponent(EntID entity) {
		return m_registry.GetComponent<T>(entity);
	}

	
	template<typename T>
	const T* Scene::GetComponent(EntID entity) const {
		return m_registry.GetComponent<T>(entity);
	}


	// Registry operations

	
	template<typename T>
	CompID Scene::RegisterComponent(void) {
		return m_registry.RegisterComponent<T>();
	}


	
	template<typename T>
	bool Scene::HasRegisteredComponent(void) const {
		return m_registry.HasRegisterComponent<T>();
	}

	
	template<typename T>
	CompID Scene::UnregisterComponent(void) {
		return m_registry.UnregisterComponent<T>();
	}


}
