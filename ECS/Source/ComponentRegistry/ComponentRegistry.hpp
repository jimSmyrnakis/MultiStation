#pragma once
#include "../ComponentArray/ComponentArray.hpp"
#include "../Components/Transform.hpp"
#include <string> 
namespace MultiStation{

	class ComponentRegistry {
	public:
		enum class Type : uint16_t {
			TRANSFORM3D = 0 
		};

	private:
		ComponentRegistry(void) noexcept;
		static void Move(ComponentRegistry* dest, ComponentRegistry* src);
		~ComponentRegistry(void) noexcept;

		// no copies
		ComponentRegistry(const ComponentRegistry& cp) noexcept = delete;
		ComponentRegistry& operator=(const ComponentRegistry& cp) noexcept = delete;
	public:

		static ComponentRegistry* Create(void);
		static void Destroy(ComponentRegistry* ptr);

		

		// Only moves
		ComponentRegistry(ComponentRegistry&& move) noexcept;
		ComponentRegistry& operator=(ComponentRegistry&& move) noexcept;
		
		// Get all Scene Component's of this type
		template<typename T>
		ComponentArray<T>* GetAllComponents(void) const {
			ComponentArray<T>* carr = nullptr;
			if constexpr (std::is_same_v<T, Transform3D>) {
				carr = m_Transforms;
			}
			else {
				// TODO : Error Message to the end user
			}
			return carr;
		}

		// Get the specific type components of this entity
		template<typename T>
		std::vector<T*> GetComponent(uint32_t entity) const {
			std::vector<T*> v;
			
			if constexpr (std::is_same_v<T, Transform3D>) {
				v = m_Transforms->GetEntityComponents(entity);
			}
			else {
				// TODO : Error Message to the end user
			}

			return v;
		}

		

		// Add new Component to the entity , if entity doesn't exist
		// new one is created with this component and its arguments
		template<typename T>
		T* AddComponent(uint32_t entity) {

			// find entity
			auto it_pair = m_Entities.find(entity);
			if (it_pair == m_Entities.end()) {
				m_Entities[entity] = "Entity"; // create new one
			}


			if constexpr (std::is_same_v<T, Transform3D>) {
				return m_Transforms->AddEntityComponent(entity);
			}
			else {
				// TODO : Error Message to the end user
			}
			return nullptr;
		}


		// Remove the Component from the entity , if component is the last 
		// the entity then remove the entity too
		template<typename T>
		void RemoveComponent(T& component) {
			

			if constexpr (std::is_same_v<T, Transform3D>) {
				m_Transforms->RemoveComponent(component);
			}
			else {
				// TODO : Error Message to the end user
			}
			return;
		}


		
		// get entity name if this entity exist's
		std::string* GetEntityName(uint32_t entity);
		const std::string* GetEntityName(uint32_t entity) const ;
		
		

	private:
		
		ComponentArray<Transform3D>* m_Transforms;
		// Types of component's
		std::vector<uint16_t>    m_Types = { (uint16_t)Type::TRANSFORM3D };
		// Names of each Component
		std::vector<std::string> m_Names = { {"Transform3D"} };
		// Map entities and Names
		std::unordered_map<uint32_t , std::string> m_Entities; 
		// Log with entities and the names of each one of them
		std::unordered_map<uint32_t, uint16_t> m_EntitiesComponentsCount;

	};

}
