#pragma once
#include <stdint.h>
#include <stddef.h>
#include <atomic>
namespace MultiStation{

	/**
	 * @author Dimitris Smyrnakis
	 * @class IComponentArray
	 * @brief An interface for ComponentArray classes, providing a common base for different component types.
	 * @details This way we ensure that all component arrays can be destroyed polymorphically , as well for each
	 * different component type we can get a unique type ID .
	 */
	class IComponentArray {
		static std::atomic<uint32_t> s_typeID;
		
	public:

		/**
		 * @brief A virtual destructor to allow for proper cleanup of derived classes. 
		 */
		virtual ~IComponentArray(void) noexcept = default;

		/**
		 * @brief Gets a unique type ID for the specified component type T.
		 * 
		 */
		template<typename T>
		static uint32_t GetID(void);

		/**
		 * @brief Helper method for registry to use in order to remove all components
		 * of a entity when it is destroyed, as the registry does not know the actual 
		 * type of the component array to call its RemoveComponent method.
		 * 
		 * @param entity
		 */
		virtual void RemoveEntity(uint32_t entity) = 0;
	};


	

	template<typename T>
	uint32_t IComponentArray::GetID(void) {
		static uint32_t id = s_typeID.fetch_add(std::memory_order_relaxed);
		return id;
	}

}
