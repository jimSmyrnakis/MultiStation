#include "Registry.hpp"

namespace MultiStation {

	ECSRegistry::~ECSRegistry(void) noexcept {
		for (auto& it_pair : m_typeComponentMap) {
			delete it_pair.second;
		}
	}

	bool ECSRegistry::HasComponent(EntID entity, CompID component) noexcept {
		return m_entityManager.HasEntityComponent(entity , component);
	}
	

	

	size_t ECSRegistry::SizeOfComponent(CompID id)  noexcept {
		if (!HasRegister(id)) return 0;
		const IComponentArray* components = (const IComponentArray*) m_typeComponentMap[id];

		return components->SizeOfComponent();
	}


	size_t ECSRegistry::Count(CompID id) noexcept {
		if (!HasRegister(id)) return 0;
		const IComponentArray* components = (const IComponentArray*)m_typeComponentMap[id];

		return components->Count();
	}
	
	void* ECSRegistry::GetComponent(CompID id , EntID entity) noexcept {
		if (!HasRegister(id)) return nullptr;

		IComponentArray* components =m_typeComponentMap[id];

		return components->GetComponentV(entity);
	}

	bool ECSRegistry::HasRegister(CompID id) const noexcept {
		return m_typeComponentMap.count(id) != 0;
	}

	
	void* ECSRegistry::GetComponents(CompID id) noexcept {
		if (!HasRegister(id)) return nullptr;

		IComponentArray* components = m_typeComponentMap[id];

		return components->GetComponentsV();
	}

	bool ECSRegistry::RemoveEntity(EntID entity) noexcept {
		bool res = m_entityManager.RemoveEntity(entity);
		if (res == false) return false;

		for (auto& it_pair : m_typeComponentMap) {
			IComponentArray* carr = it_pair.second;
			carr->RemoveComponentV(entity);

		}

		return true;
	}

	
	EntID ECSRegistry::CreateEntity(void) noexcept {
		return m_entityManager.CreateEntity();
	}

	
	bool ECSRegistry::HasEntity(EntID entity) const noexcept {
		return m_entityManager.HasEntity(entity);
	}

	
	std::span<const EntID> ECSRegistry::GetAllEntities(void) const noexcept {
		return m_entityManager.GetAllEntities();
	}

	std::span<const CompID> ECSRegistry::GetSignatureOf(EntID entity) const noexcept {
		return m_entityManager.GetSignatureOf(entity);
	}

	
	bool ECSRegistry::RemoveComponent(EntID entity, CompID component) noexcept {
		if (
			!HasRegister( component)
			||
			!m_entityManager.HasEntity(entity)
			) {
			return false;
		}

		IComponentArray* components = 
			m_typeComponentMap[component];
		m_entityManager.RemoveComponentOf(entity , component);
		components->RemoveComponentV(entity);




		return true;
	}


	void* ECSRegistry::AddComponent(EntID entity, CompID component) noexcept{
		if (
			!HasRegister(component)
			||
			!m_entityManager.HasEntity(entity)
			) {
			return nullptr;
		}

		IComponentArray* components = m_typeComponentMap[component];

		void* result = components->AddComponentV(entity);
		if (result == nullptr) {
			return nullptr;
		}

		m_entityManager.AssignToEntity(entity , component);

		return result;
	}


	void* ECSRegistry::ReplaceComponent(EntID entity, CompID component) noexcept{
		if (
			!HasRegister(component)
			||
			!m_entityManager.HasEntity(entity)
			||
			!m_entityManager.HasEntityComponent(entity ,component )
			) {
			return nullptr;
		}

		IComponentArray* components = m_typeComponentMap[component];

		return components->ReplaceComponentV(entity);
	}

	std::string nullName = "none";
	
	const std::string& ECSRegistry::GetName(CompID component) const noexcept {
		if (!HasRegister(component)) {
			return nullName;
		}

		const IComponentArray* components = m_typeComponentMap.at(component);

		return components->GetName();
	}

}
