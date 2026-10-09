#include "Registry.hpp"

namespace MultiStation {

	Registry::~Registry(void) noexcept {
		for (auto& it_pair : m_typeComponentMap) {
			delete it_pair.second;
		}
	}

	

	size_t Registry::SizeOfComponent(CompID id)  {
		if (!HasRegister(id)) return 0;
		const IComponentArray* components = (const IComponentArray*) m_typeComponentMap[id];

		return components->SizeOfComponent();
	}


	size_t Registry::Count(CompID id) {
		if (!HasRegister(id)) return 0;
		const IComponentArray* components = (const IComponentArray*)m_typeComponentMap[id];

		return components->Count();
	}
	
	void* Registry::GetComponent(CompID id , EntID entity) {
		if (!HasRegister(id)) return nullptr;

		IComponentArray* components =m_typeComponentMap[id];

		return components->GetComponentV(entity);
	}

	bool Registry::HasRegister(CompID id) {
		return m_typeComponentMap.count(id) != 0;
	}

	
	void* Registry::GetComponents(CompID id) {
		if (!HasRegister(id)) return nullptr;

		IComponentArray* components = m_typeComponentMap[id];

		return components->GetComponentsV();
	}

	bool Registry::RemoveEntity(EntID entity) {
		bool res = m_entityManager.RemoveEntity(entity);
		if (res == false) return false;

		for (auto& it_pair : m_typeComponentMap) {
			IComponentArray* carr = it_pair.second;
			carr->RemoveComponentV(entity);

		}

		return true;
	}

	
	EntID Registry::CreateEntity(void) {
		return m_entityManager.CreateEntity();
	}

	
	bool Registry::HasEntity(EntID entity) const {
		return m_entityManager.HasEntity(entity);
	}

	
	std::span<const EntID> Registry::GetAllEntities(void) const {
		return m_entityManager.GetAllEntities();
	}

	CompID Registry::GetSignatureOf(EntID entity) const {
		return m_entityManager.GetSignatureOf(entity);
	}

	
	

	

}
