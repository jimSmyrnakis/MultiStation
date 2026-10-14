#include "Scene.hpp"

namespace MultiStation {
	static SceneSerializer scene_serializer;


	

	SceneSerializer& GetSceneSerializer(void) {
		return scene_serializer;
	}
	Scene::Scene(EngineContext& context,SerializationRegistry& registry , ObjectMap& objMap)
		: m_engineContext(context) , m_serializationRegistry(registry) , m_objectMap(objMap){
		
		
		m_serializationRegistry.
			RegisterObject(GetObjectID<Scene>(), &scene_serializer);
	}
	
	bool Scene::RemoveEntity(EntID entity) {
		if (!m_registry.HasEntity(entity)) {
			return false;
		}
		
		if (!m_sceneGraph.RemoveEntity(entity)) {
			return false;
		}

		return m_registry.RemoveEntity(entity);
	}

	
	EntID Scene::CreateEntity(void) {
		EntID new_ent = m_registry.CreateEntity();
		m_sceneGraph.AddEntity(new_ent);
		return new_ent;
	}

	
	bool Scene::HasEntity(EntID entity) const {
		return m_registry.HasEntity(entity); 
	}

	
	std::span<const EntID> Scene::GetAllEntities(void) const {
		return m_registry.GetAllEntities();
	}

	
	bool  Scene::SetParent(EntID entity, EntID parent) {
		if (!m_registry.HasEntity(entity) || 
			((parent != rootEntity) && !m_registry.HasEntity(parent) ) ) {
			return false;
		}
		return m_sceneGraph.SetParent(entity, parent);
	}

	
	bool Scene::GetParent(EntID entity, EntID& out) const {
		if (!m_registry.HasEntity(entity) ) {
			return false;
		}
		return m_sceneGraph.GetParent(entity, out);
	}

	
	std::span<const EntID> Scene::GetChildren(EntID entity) const {
		if (!m_registry.HasEntity(entity) && (entity != rootEntity) ) {
			return {};
		}
		return m_sceneGraph.GetChilds(entity);
	}


	// Untemplated types
	
	void* Scene::GetComponent(CompID component, EntID entity) {
		return m_registry.GetComponent(component, entity);
	}

	
	void* Scene::GetComponents(CompID component) {
		return m_registry.GetComponents(component);
	}

	
	bool Scene::HasRegister(CompID component) {
		return m_registry.HasRegister(component);
	}

	
	size_t Scene::SizeOfComponent(CompID component) {
		return m_registry.SizeOfComponent(component);
	}

	
	size_t Scene::Count(CompID component) {
		return m_registry.Count(component);
		
	}


	
	bool Scene::HasComponent(EntID entity, CompID component) {
		return m_registry.HasComponent(entity, component);
	}

	std::span< const CompID> Scene::GetEntityComponentsIDs(EntID entity) const {
		return m_registry.GetSignatureOf(entity);
	}

	bool Scene::GetComponentObjectID(CompID component_id, ObjID& object_id) {
		return m_objectMap.GetComponentObjectMap(component_id, object_id);
	}
}
