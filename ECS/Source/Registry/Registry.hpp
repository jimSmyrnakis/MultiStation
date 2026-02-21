#pragma once
#include "../ComponentArray/ComponentArray.hpp"
#include <Platform.hpp>
namespace MultiStation {

	/**
	 * @author Dimitris Smyrnakis
	 * @class Registry
	 * @brief The Registry class manages the registration and retrieval of component arrays
	 * for different component types in an ECS architecture. Each component type T has its own
	 * ComponentArray <T> instance, which is created upon registration and destroyed upon unregistration.
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

	public: // get components


		/**
		 * @brief Gets the ComponentArray for the specified component type T 
		 * if is registered in the Registry.
		 * @tparam T The type of the component array to retrieve.
		 * @returns A pointer to the ComponentArray<T> if registered, nullptr otherwise.
		 */
		template<typename T>
		ComponentArray<T>* GetComponentArray(void) const;


		void RemoveEntity(uint32_t entity);
		



	public: // 

		/**
		 * @brief Registers a new ComponentArray for the specified component type T
		 * if this Component doesn't exist or otherwise does nothing .
		 * @tparam T The type of the component array to register.
		 * @returns ID of the registered component type
		 */
		template<typename T>
		uint32_t Register(void);


		/**
		 * @brief Checks if a ComponentArray for the specified component type T is registered .
		 * @tparam T The type of the component array to check.
		 * @returns true if registered, false otherwise.
		 */
		template<typename T>
		bool HasRegister(void) const;

		/**
		 * @brief Unregisters the ComponentArray for the specified component type T if is registered,
		 * otherwise does nothing.
		 * @tparam T The type of the component array to unregister.
		 * @returns ID of the unregistered component type
		 */
		template<typename T>
		uint32_t Unregister(void);

	private:
		std::unordered_map<uint32_t, IComponentArray*> m_typeComponentMap;
		// componentArray<T> object and type ==> T for each component type
		// registration



	};

	

	template<typename T>
	ComponentArray<T>* Registry::GetComponentArray(void) const {
		const uint32_t id = IComponentArray::GetID<T>();
		auto it = m_typeComponentMap.find(id);
		if (it == m_typeComponentMap.end()) {
			MS_ASSERT(false, "Component type not registered in the registry.");
			return nullptr;
		}
		return static_cast<ComponentArray<T>*>(it->second);
	}


	template<typename T>
	uint32_t Registry::Register(void) {
		uint32_t id = ComponentArray<T>::GetID();
		if (HasRegister<T>()) {
			MS_ASSERT(false, "Component type already registered in the registry.");
			return id;
		}
		
		// else register that
		ComponentArray<T>* carr = new(std::nothrow) ComponentArray<T>();
		if (carr == nullptr) {
			MS_ASSERT(false, "Failed to allocate memory for ComponentArray.");
			return id;
		}
		m_typeComponentMap[id] = carr;

		return id;
	}

	template<typename T>
	bool Registry::HasRegister(void) const {
		uint32_t id = IComponentArray::GetID<T>();
		return m_typeComponentMap.count(id) != 0;
	}


	template<typename T>
	uint32_t Registry::Unregister(void) {
		if (!HasRegister<T>()) {
			MS_ASSERT(false, "Component type not registered in the registry.");

			return MultiStation::BAD_ID;
		}
		// else unregister that
		uint32_t id = ComponentArray<T>::GetID();
		// delete it first
		auto it = m_typeComponentMap.find(id);
		if (it != m_typeComponentMap.end()) { 
			delete it->second; m_typeComponentMap.erase(it); 
		}
		return id;
	}


	

}
