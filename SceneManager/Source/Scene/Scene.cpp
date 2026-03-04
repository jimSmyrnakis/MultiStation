#include "Scene.hpp"

namespace MultiStation {
	Scene::Scene(ECSManager& context) noexcept : m_context(context) {
		m_nextGameObjectID = 0;
		m_context.Register<EntityInfo>();

		m_gameObjects.reserve(1000);
		m_gameObjectIDToIndex.reserve(1000);
	}
	Scene::~Scene(void) noexcept {
		
	}


	GameObject* Scene::CreateGameObject(const std::string& name) noexcept {
		uint32_t id = m_nextGameObjectID;
		m_gameObjects.emplace_back(m_context, id);
		GameObject& gameObject = m_gameObjects.back();
		m_gameObjectIDToIndex[id] = m_gameObjects.size() - 1;
		gameObject.SetName(name.c_str(), (uint8_t)std::min(name.size() + 1, (size_t)63));
		m_context.CreateEntity(id);
		//m_context.AddComponent<EntityInfo>(id, name);
		m_nextGameObjectID++;
		return &gameObject;
	}

	void Scene::RemoveGameObject(GameObject* gameObject) noexcept {
		if (!gameObject) return;

		uint32_t id = gameObject->GetID();
		auto it = m_gameObjectIDToIndex.find(id);
		if (it == m_gameObjectIDToIndex.end()) return;

		size_t index = it->second;
		size_t lastIndex = m_gameObjects.size() - 1;

		// swap
		std::swap(m_gameObjects[index], m_gameObjects[lastIndex]);

		// update moved object's index
		m_gameObjectIDToIndex[m_gameObjects[index].GetID()] = index;

		// remove last
		m_gameObjects.pop_back();
		m_gameObjectIDToIndex.erase(it);

		m_context.DestroyEntity(id);
	}

	GameObject* Scene::GetGameObjectByID(uint32_t id) noexcept {
		auto it = m_gameObjectIDToIndex.find(id);
		if (it == m_gameObjectIDToIndex.end()) {
			MS_ENGINE_WARN("GameObject not found in the scene. ");
			return nullptr;
		}
		return m_gameObjects.data() + it->second;
	}

	const GameObject* Scene::GetGameObjectByID(uint32_t id) const noexcept {
		auto it = m_gameObjectIDToIndex.find(id);
		if (it == m_gameObjectIDToIndex.end()) {
			MS_ENGINE_WARN("GameObject not found in the scene. ");
			return nullptr;
		}
		return m_gameObjects.data() + it->second;
	}

	std::vector<GameObject>& Scene::GetGameObjects(void) noexcept {
		return m_gameObjects;
	}

	const std::vector<GameObject>& Scene::GetGameObjects(void) const noexcept {
		return m_gameObjects;
	}


	std::string Scene::GetSceneName(void) const noexcept {
		return m_sceneName;
	}

	void Scene::SetSceneName(const std::string& name) noexcept {
		m_sceneName = name;
	}

	std::vector<std::string> Scene::GetRegisteredComponentTypes(void) const noexcept {
		return m_registeredComponentTypes;
	}

	void Scene::UpdateScene(void) noexcept {
		m_context.PollOperations();
	}
}
