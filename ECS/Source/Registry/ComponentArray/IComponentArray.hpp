#pragma once
#include <stdint.h>
#include <stddef.h>
#include "../../Globals/Globals.hpp"
#include <atomic>
namespace MultiStation {

	/**
	 * @author Dimitris Smyrnakis
	 * @class IComponentArray
	 * @brief An interface for ComponentArray classes, providing a common base for different component types.
	 * @details This way we ensure that all component arrays can be accessed polymorphically , as well for each
	 * different component type (templates) .
	 */
	class IComponentArray {
		

	public:

		/**
		 * @brief A virtual destructor to allow for proper cleanup of derived classes.
		 */
		virtual ~IComponentArray(void) noexcept = default;

		

		/**
		 * @brief Helper method of removing component
		 *
		 * @param entity the Entity id
		 */
		virtual void RemoveComponentV(EntID entity) = 0;

		/**
		 * @brief Virtual method of adding components 
		 * 
		 * @param entity The Entity id
		 * @return pointer to the new added component if succeded , nullptr otherwise  
		 */
		virtual void* AddComponentV(EntID entity) = 0;

		/**
		 * @brief Virtual method of replacing existing components
		 *
		 * @param entity The Entity id
		 * @return pointer to the new replaced component if succeded , nullptr otherwise
		 */
		virtual void* ReplaceComponentV(EntID entity) = 0;

		/**
		 * @brief Virtual method of checking if the entity has a component
		 * 
		 * @param entity The entity id
		 * @return true if has a component , false otherwise .  
		 */
		virtual bool HasComponentV(EntID entity) = 0;

		/**
		 * @brief Virtual method for getting component of a entity or nullptr if not has one .
		 * 
		 * @param entity Entity id.
		 * 
		 */
		virtual void* GetComponentV(EntID entity) = 0;

		/**
		 * @brief Virtual Method for getting all components .
		 *
		 * @param entity Entity id.
		 *
		 */
		virtual void* GetComponentsV(void) =0;

		/**
		 * @brief Virtual method that returns the size of each component in bytes 
		 */
		virtual size_t SizeOfComponent(void) const = 0;

		/**
		 * @brief Virtual method that returns the number of components . 
		 */
		virtual size_t Count(void) const = 0;

	};




	

}

