#include "Scene.hpp"

namespace MultiStation {
	
	Scene::Scene(EngineContext& context , ObjectMap& objMap) noexcept
		: m_engineContext(context)  , m_objectMap(objMap){
		
		
	}
	
	bool Scene::RemoveEntity(EntID entity) noexcept {
		if (!m_ecsRegistry.HasEntity(entity)) {
			return false;
		}
		
		if (!m_sceneGraph.RemoveEntity(entity)) {
			return false;
		}

		return m_ecsRegistry.RemoveEntity(entity);
	}

	
	EntID Scene::CreateEntity(void) noexcept {
		EntID new_ent = m_ecsRegistry.CreateEntity();
		m_sceneGraph.AddEntity(new_ent);
		
		return new_ent;
	}

	
	bool Scene::HasEntity(EntID entity) const noexcept {
		return m_ecsRegistry.HasEntity(entity);
	}

	
	std::span<const EntID> Scene::GetAllEntities(void) const noexcept {
		return m_ecsRegistry.GetAllEntities();
		
	}

	
	bool  Scene::SetParent(EntID entity, EntID parent) noexcept {
		if (!m_ecsRegistry.HasEntity(entity) ||
			((parent != rootEntity) && !m_ecsRegistry.HasEntity(parent) ) ) {
			return false;
		}
		return m_sceneGraph.SetParent(entity, parent);
	}

	
	bool Scene::GetParent(EntID entity, EntID& out) const noexcept {
		if (!m_ecsRegistry.HasEntity(entity) ) {
			return false;
		}
		return m_sceneGraph.GetParent(entity, out);
	}

	
	std::span<const EntID> Scene::GetChildren(EntID entity) const noexcept {
		if (!m_ecsRegistry.HasEntity(entity) && (entity != rootEntity) ) {
			return {};
		}
		return m_sceneGraph.GetChilds(entity);
	}


	// Untemplated types
	
	void* Scene::GetComponent(CompID component, EntID entity) noexcept {
		return m_ecsRegistry.GetComponent(component, entity);
	}

	
	void* Scene::GetComponents(CompID component) noexcept {
		return m_ecsRegistry.GetComponents(component);
	}

	
	bool Scene::HasRegister(CompID component) const noexcept {
		return m_ecsRegistry.HasRegister(component);
	}

	
	size_t Scene::SizeOfComponent(CompID component) noexcept {
		return m_ecsRegistry.SizeOfComponent(component);
	}

	
	size_t Scene::Count(CompID component) noexcept {
		return m_ecsRegistry.Count(component);
		
	}


	
	bool Scene::HasComponent(EntID entity, CompID component) noexcept {
		return m_ecsRegistry.HasComponent(entity, component);
	}

	std::span< const CompID> Scene::GetEntityComponentsIDs(EntID entity) const noexcept {
		return m_ecsRegistry.GetSignatureOf(entity);
	}

	bool Scene::GetComponentObjectID(CompID component_id, ObjID& object_id) noexcept {
		return m_objectMap.GetComponentObjectMap(component_id, object_id);
	}



	bool Scene::RemoveComponent(EntID entity, CompID component) noexcept {
		return m_ecsRegistry.RemoveComponent(entity, component);
	}

	void* Scene::AddComponent(EntID entity, CompID component) noexcept {
		return m_ecsRegistry.AddComponent(entity, component);
	}

	void* Scene::ReplaceComponent(EntID entity, CompID component) noexcept {
		return m_ecsRegistry.ReplaceComponent(entity, component);
	}


	const std::string& Scene::GetName(CompID component) const noexcept {
		return m_ecsRegistry.GetName(component);
	}

	CompID Scene::GetComponentIDByName(std::string& name) {
		return m_ecsRegistry.GetComponentIDByName(name);
	}
}
