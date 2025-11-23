#pragma once
#include <vector>
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <stdint.h>
#include <stddef.h>

namespace MultiStation{
	template<class T>
	class ComponentArray {

	protected:
		ComponentArray(bool isUnique = false) noexcept {
			m_Unique = isUnique;
			
		};
		~ComponentArray(void) noexcept {
			ClearVectors();
		}
		
		ComponentArray(const ComponentArray& copy) = delete;
		ComponentArray& operator=(const ComponentArray& copy) = delete;

		void ClearVectors(void) {
			m_data.clear();
			m_entitiesToData.clear();
			m_entityToComponentsIndexes.clear();
		}
		
		void Move(ComponentArray* dest, ComponentArray* src) {
			if (dest == src) return;


			dest->ClearVectors();
			dest->m_data = std::move(src->m_data);
			dest->m_entitiesToData = std::move(src->m_entitiesToData);
			dest->m_entityToComponentsIndexes = std::move(src->m_entityToComponentsIndexes);
			dest->m_Unique = src->m_Unique;
			src->ClearVectors();
		}

		ComponentArray(ComponentArray&& move) {
			Move(this, &move);

		}
		ComponentArray& operator=(ComponentArray&& move) {
			if (this != &move)
				Move(this, &move);

			return *this;
		}

	public:

		static ComponentArray* Create(bool isUnique = false) {
			return new ComponentArray(isUnique);
		}

		static void Destroy(ComponentArray* ptr) {
			if (ptr)
				delete ptr;
		}

		// Create a new Component for this entity , check's if is Unique
		// if is Unique and there is a entity here then send's warning
		// and does nothing 
		T* AddEntityComponent(uint32_t entity) {

			// do we have this entity here ?
			if (m_Unique 
				&& 
				m_entityToComponentsIndexes.find(entity) != m_entityToComponentsIndexes.end())
				return nullptr;


			// if every check succeded then 
			// add the entity matching the component index 
			m_entitiesToData.push_back(entity);
			// create the new component by passing the variable length arguments
			m_data.emplace_back(); // no parameters 
			T* res = &m_data.at(m_data.size() - 1);
			// add this component to the unordered map 
			m_entityToComponentsIndexes[entity].push_back(m_data.size() - 1);
			return res;
		}

		// Removes the component if it finds it or do nothing if not find it
		void RemoveComponent(T& component) {
			// search the component
			auto it = std::find(m_data.begin(), m_data.end(), component);
			if (it == m_data.end()) {
				// do nothing , this component is not here
				// TODO : Give Error to the User for that 
				return;
			}

			// if exist's , then remove it from the entity etirly
			size_t index = std::distance(m_data.begin(), it);
			uint32_t entity = m_entitiesToData[index];
			std::vector<size_t>& entityComponents = m_entityToComponentsIndexes[entity];
			// search it in the entity components
			auto it2 = entityComponents.begin();
			while (it2 != entityComponents.end()) {
				if (m_data[(*it2)] == component) {
					entityComponents.erase(it2);
					break;
				}
				it2++; // it2 is the index to the m_data
			}
			// TODO : if not found then fatal error


			m_data.erase(it);
			m_entitiesToData.erase(m_entitiesToData.begin() + index);
			// TODO : Rest Elements after it must go one position back
			// Warning the Map to must be check for that . Big Bug !!!!
		}

		
		std::vector<T*> GetEntityComponents(uint32_t entity) {
			std::vector<T*> entities;

			auto it = m_entityToComponentsIndexes.find(entity);
			if (it != m_entityToComponentsIndexes.end()) {
				for (size_t index : it->second)  // it->second είναι το vector<size_t>
					entities.push_back(&m_data[index]);
			}

			return entities;
		}
		
		// Get Entity from the component , if exist in this Component Array
		bool GetEntity(T& component , uint32_t& entity) {
			
			auto it = std::find(m_data.begin(), m_data.end(), component);
			if (it != m_data.end()) {
				// if found
				// return the entity
				size_t index = std::distance(m_data.begin(), it);
				entity = m_entitiesToData[index];
				return true;
			}
			
			// else not found 
			return false;
		}

		bool HasEntity(uint32_t entity) {
			auto it_pair = m_entityToComponentsIndexes.find(entity);
			return it_pair != m_entityToComponentsIndexes.end();
		}


		// Remove All Entity Components
		void RemoveEntityComponents(uint32_t entity) {
			auto it_pair = m_entityToComponentsIndexes.find(entity);
			if (it_pair == m_entityToComponentsIndexes.end()) {
				return; // entity δεν υπάρχει
			}

			std::vector<size_t>& componentIndexes = it_pair->second;

			// Reverse remove for keeping order
			for (auto it = componentIndexes.rbegin(); it != componentIndexes.rend(); ++it) {
				size_t index = *it;
				m_data.erase(m_data.begin() + index);
				m_entitiesToData.erase(m_entitiesToData.begin() + index);
			}

			// remove the unordered pair from the unordered map 
			m_entityToComponentsIndexes.erase(it_pair);
		}
		

	private:
		std::vector<T> m_data; // Components Data all together - Cache friendly
		std::vector<uint32_t> m_entitiesToData; // what entity each component belongs ?
		std::unordered_map<uint32_t,std::vector< size_t>> m_entityToComponentsIndexes; // save each entity 
		// the components index in the m_data component list
		bool m_Unique; // Can each entity has multiple same type components or not
		// This get scense for Scripts especially


		
	};




}
