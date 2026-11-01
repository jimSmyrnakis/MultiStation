#include "Registry.hpp"

namespace MultiStation {

	ECSRegistry::ECSRegistry(void) noexcept {

		MS_ASSERT(CentralStorageManager::CreateStorage(m_storageID , 
			[this](CompID id) {
				for (auto ent : m_entityManager.GetAllEntities()) {
					if (m_entityManager.HasEntityComponent(ent , id))
						m_entityManager.RemoveComponentOf(ent, id);
				}
			}), "No Available storage");
		m_storage = CentralStorageManager::GetStorage(m_storageID);
		MS_ASSERT(m_storage != nullptr, "No available storage");
	}


	ECSRegistry::~ECSRegistry(void) noexcept {
		m_storage = nullptr;
		CentralStorageManager::DestroyStorage(m_storageID);
	}

	bool ECSRegistry::HasComponent(EntID entity, CompID component) noexcept {
		return m_entityManager.HasEntityComponent(entity , component);
	}
	

	

	size_t ECSRegistry::SizeOfComponent(CompID id)  noexcept {
		if (!HasRegister(id)) return 0;
		const IComponentArray* components = (const IComponentArray*) m_storage->components[id];

		return components->SizeOfComponent();
	}


	size_t ECSRegistry::Count(CompID id) noexcept {
		if (!HasRegister(id)) return 0;
		const IComponentArray* components = (const IComponentArray*)m_storage->components[id];

		return components->Count();
	}
	
	void* ECSRegistry::GetComponent(CompID id , EntID entity) noexcept {
		if (!HasRegister(id)) return nullptr;

		IComponentArray* components = m_storage->components[id];

		return components->GetComponentV(entity);
	}

	bool ECSRegistry::HasRegister(CompID id) const noexcept {
		return CentralStorageManager::HasRegister(id);
	}

	
	void* ECSRegistry::GetComponents(CompID id) noexcept {
		if (!HasRegister(id)) return nullptr;

		IComponentArray* components = m_storage->components[id];

		return components->GetComponentsV();
	}

	bool ECSRegistry::RemoveEntity(EntID entity) noexcept {
		bool res = m_entityManager.RemoveEntity(entity);
		if (res == false) return false;

		for (auto& it_pair : m_storage->components) {
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
			m_storage->components[component];
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

		IComponentArray* components = m_storage->components[component];

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

		IComponentArray* components = m_storage->components[component];

		return components->ReplaceComponentV(entity);
	}

	std::string nullName = "none";
	
	const std::string& ECSRegistry::GetName(CompID component) const noexcept {
		if (!HasRegister(component)) {
			return nullName;
		}

		const IComponentArray* components = m_storage->components.at(component);

		return components->GetName();
	}

	CompID ECSRegistry::GetComponentIDByName(std::string& name) {
		for (auto it = m_storage->components.begin(); it != m_storage->components.end(); it++) {
			if (it->second->GetName() == name)
				return it->first;
		}
		return 0;
	}

}
