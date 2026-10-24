#pragma once
#include <stdint.h>
#include <stddef.h>
#include "../../Globals/Globals.hpp"
#include <atomic>
/**
 * @author Dimitris Smyrnakis
 * @file IComponentArray.hpp
 * @brief This file contains the IComponentArray interface 
 * that helps the templated component array to override same 
 * methods untemplated and gives the power to registry to handle 
 * a lot component arrays and component id relations .
 * @version 0.1
 */
namespace MultiStation {

	/**
	 * 
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
		virtual void RemoveComponentV(EntID entity) noexcept = 0;

		/**
		 * @brief Virtual method of adding components 
		 * 
		 * @param entity The Entity id
		 * @return pointer to the new added component if succeded , nullptr otherwise  
		 */
		virtual void* AddComponentV(EntID entity) noexcept = 0;

		/**
		 * @brief Virtual method of replacing existing components
		 *
		 * @param entity The Entity id
		 * @return pointer to the new replaced component if succeded , nullptr otherwise
		 */
		virtual void* ReplaceComponentV(EntID entity)noexcept = 0;

		/**
		 * @brief Virtual method of checking if the entity has a component
		 * 
		 * @param entity The entity id
		 * @return true if has a component , false otherwise .  
		 */
		virtual bool HasComponentV(EntID entity)noexcept = 0;

		/**
		 * @brief Virtual method for getting component of a entity or nullptr if not has one .
		 * 
		 * @param entity Entity id.
		 * 
		 */
		virtual void* GetComponentV(EntID entity)noexcept = 0;

		/**
		 * @brief Virtual Method for getting all components .
		 *
		 * @param entity Entity id.
		 *
		 */
		virtual void* GetComponentsV(void)noexcept =0;

		/**
		 * @brief Virtual method that returns the size of each component in bytes 
		 */
		virtual size_t SizeOfComponent(void) const noexcept = 0;

		/**
		 * @brief Virtual method that returns the number of components . 
		 */
		virtual size_t Count(void) const noexcept = 0;

		/**
		 * @returns The name of the component type  
		 */
		virtual const std::string& GetName(void) const noexcept = 0;

	};




	

}

