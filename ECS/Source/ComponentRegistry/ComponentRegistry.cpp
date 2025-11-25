#include "ComponentRegistry.hpp"
#include <string.h>
namespace MultiStation {

	

	ComponentRegistry::ComponentRegistry(void) noexcept {
		m_Transforms = ComponentArray<Transform3D>::Create(false);

	}
	void ComponentRegistry::Move(ComponentRegistry* dest, ComponentRegistry* src) {
		// check if the same pointer's go for move operation
		if (dest == src) return;
		
		// remove and free all destination old elements
		ComponentArray<Transform3D>::Destroy(dest->m_Transforms);
		dest->m_Entities.clear();
		dest->m_Names.clear();
		dest->m_Types.clear();

		// move source elements to destination
		dest->m_Entities = std::move(src->m_Entities);
		dest->m_Names = std::move(src->m_Names);
		dest->m_Transforms = src->m_Transforms;
		dest->m_Types = std::move(src->m_Types);

		// clear all source elements but free nothing
		src->m_Entities.clear();
		src->m_Names.clear();
		src->m_Transforms = nullptr;
		src->m_Types.clear();

	}

	ComponentRegistry::~ComponentRegistry(void) noexcept {
		ComponentArray<Transform3D>::Destroy(m_Transforms);
		m_Entities.clear();
		m_Names.clear();
		m_Types.clear();
		m_Transforms = nullptr;
		
	}

	ComponentRegistry* ComponentRegistry::Create(void) {
		return new ComponentRegistry();
	}

	void ComponentRegistry::Destroy(ComponentRegistry* ptr) {
		if (ptr)
			delete ptr;
	}

	// Only moves
	ComponentRegistry::ComponentRegistry(ComponentRegistry&& move) noexcept {
		this->Move(this, &move);
	}
	ComponentRegistry& ComponentRegistry::operator=(ComponentRegistry&& move) noexcept {
		this->Move(this, &move);
		return *this;
	}


































	// get entity name if this entity exist's
	std::string* ComponentRegistry::GetEntityName(uint32_t entity) {
		auto it = m_Entities.find(entity);
		if (it == m_Entities.end()) {
			return nullptr;  // entity not found
		}
		std::string& st = it->second;
		return &st;  // επιστρέφουμε pointer στο string

	}

	const std::string* ComponentRegistry::GetEntityName(uint32_t entity) const {
		const auto it = m_Entities.find(entity);
		if (it == m_Entities.end()) {
			return nullptr;  // entity not found
		}
		const std::string& st = it->second;
		return &st;  // επιστρέφουμε pointer στο string

	}







}
