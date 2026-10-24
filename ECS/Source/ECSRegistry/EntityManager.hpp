#pragma once
#include "../Globals/Globals.hpp"
#include <vector>
#include <span>
/**
 * @file EntityManager.hpp
 * @author Dimitris Smyrnakis
 * @brief Has the class EntityManager that is the source of truth for 
 * what entities exist's .
 * @version 0.1
 */
namespace MultiStation {

	/**
	 * @class EntityManager
	 * @brief The source of truth for what entities exist's in the engine 
	 * scene .
	 * @warning Not thread-safe.
	 * @note Unexpected exceptions terminate the application.
	 * @note This manager is for use from the ecs registry only .
	 */
	class EntityManager {
	public:

		/**
		 * @brief EntityManager constructor
		 * 
		 */
		EntityManager(void) noexcept;

		/**
		 * @returns A new Entity id (number) for the new entity that has been created 
		 * or a nullEntity id if there is no other available id to be given;
		 */
		EntID CreateEntity(void) noexcept;

		/**
		 * @param entity The entity id to remove
		 * @return true if the entity removed succsfully , false otherwise 
		 */
		bool RemoveEntity(EntID entity) noexcept;

		/**
		 * 
		 * @param entity The entity id
		 * @return true if entity with this id exist's , false otherwise .
		 */
		bool HasEntity(EntID entity) const noexcept;

		/**
		 * @brief Assigns a component id to the specify entity id
		 * @param entity The entity id to assign a component too
		 * @param component The component id for the entity to assigned
		 * @return True if entity exists false otherwise . Even if the 
		 * component is assigned already .
		 */
		bool AssignToEntity(EntID entity, CompID component) noexcept;

		/** 
		 * @param entity The entity id
		 * @param component The component id
		 * @returns true if entity has component assigned , false otherwise 
		 */
		bool HasEntityComponent(EntID entity, CompID component) const noexcept;

		
		/**
		 * @brief Removes a component assingment from this entity .
		 * @param entity The entity id
		 * @param component The component id
		 * @return True if entity exist's , false otherwise . Even if the 
		 * component is removed already
		 */
		bool RemoveComponentOf(EntID entity, CompID component) noexcept;

		/**
		 * @returns A span no owning view with all available entities in the 
		 * manager .
		 * @note Do not add / remove entities will using the span , otherwise it may
		 * no longer be valid . 
		 */
		std::span<const EntID> GetAllEntities(void) const noexcept;

		/**
		 * @param entity The entity id
		 * @returns A span no owning view with all assigned components for this entity
		 * , if the entity exist's otherwise is an empty storage .
		 * @note Do not assign / remove this entity components while using this span , otherwise
		 * it may no longer be valid storage . 
		 */
		std::span<const CompID> GetSignatureOf(EntID entity) const noexcept;

		/**
		 * @brief A templated version of the AssignToEntity(entity , component) 
		 * @tparam T The component class/primitive type 
		 * @param entity The entity id.
		 * @returns true if entity exist's , false otherwise 
 		 */
		template<typename T>
		bool AssignToEntity(EntID entity) noexcept;

		/**
		 * @brief A templated version of the HasEntityComponent(entity , component)
		 * @tparam T The component class/primitive type
		 * @param entity The entity id.
		 * @returns true if entity exist's and has the component T assigned, false otherwise
		 */
		template<typename T>
		bool HasEntityComponent(EntID entity) const noexcept;

		/**
		 * @brief A templated version of the RemoveComponentOf(entity , component)
		 * @tparam T The component class/primitive type
		 * @param entity The entity id.
		 * @returns true if entity exist's , false otherwise
		 */
		template<typename T>
		bool RemoveComponentOf(EntID entity) noexcept;

		


	private:
		std::vector<EntID> m_entities;
		std::vector<std::vector<CompID>> m_signature;
		EntID m_counter;
	};










	template<typename T>
	bool EntityManager::AssignToEntity(EntID entity) noexcept {
		return AssignToEntity(entity, GetComponentID<T>());
	}



	template<typename T>
	bool EntityManager::HasEntityComponent(EntID entity) const noexcept {
		return this->HasEntityComponent(entity, GetComponentID<T>());
	}

	template<typename T>
	bool EntityManager::RemoveComponentOf(EntID entity) noexcept {
		return RemoveComponentOf(entity, GetComponentID<T>());
	}

}
