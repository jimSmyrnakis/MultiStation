#include "Scene.hpp"

namespace MultiStation {
	Scene::Scene(void) noexcept  {
		m_nextGameObjectID = 0;
		m_context.Register<EntityInfo>();
		m_context.Register<Transform>();
		m_gameObjects.reserve(1000);
		m_gameObjectIDToIndex.reserve(1000);
	}
	Scene::~Scene(void) noexcept {
		
	}


	GameObject* Scene::CreateGameObject(const std::string& name) noexcept {
		uint32_t id = m_nextGameObjectID;
		if (!m_freelist.empty()) {
			int index = m_freelist.back();
			m_freelist.pop_back();
			m_gameObjects[index].SetActive(true);
			id = m_gameObjects[index].GetID();
			m_gameObjectIDToIndex[id] = index;
			m_gameObjects[index].SetName(name.c_str(), (size_t)name.size());
			m_context.CreateEntity(id);
			m_context.AddComponent<Transform>(id);
			return &m_gameObjects[index];
		}
		m_gameObjects.emplace_back(m_context, id);
		GameObject& gameObject = m_gameObjects.back();
		m_gameObjectIDToIndex[id] = m_gameObjects.size() - 1;
		gameObject.SetName(name.c_str(), (uint8_t)std::min(name.size() + 1, (size_t)63));
		m_context.CreateEntity(id);
		m_context.AddComponent<Transform>(id);
		m_nextGameObjectID++;
		return &gameObject;
	}

	void Scene::RemoveGameObject(GameObject* gameObject) noexcept {
		if (!gameObject) {
			MS_ENGINE_WARN("Game Object is null");
			return;
		}

		if (gameObject->IsActive() == false) {
			MS_ENGINE_WARN("Game Object is already removed !");
			return;
		}


		uint32_t id = gameObject->GetID();
		if (gameObject->GetID() == m_gameObjects[m_gameObjects.size() - 1].GetID()) {
			m_gameObjects.pop_back();
		}
		else {
			m_freelist.push_back(m_gameObjectIDToIndex[id]);
			m_gameObjects[m_gameObjectIDToIndex[id]].SetActive(false);
		}

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

	void Scene::ForEachGameObject(std::function<void(GameObject& object)> callback, uint32_t max_count) noexcept {
		if (callback == nullptr) {
			MS_ENGINE_WARN("No callback is given");
			return;
		}
		if (max_count == 0) max_count = 0xFFFFFFFF;
		uint32_t cnt = 0;
		for (GameObject& gameobject : m_gameObjects) {
			if (gameobject.IsActive()) {
				callback(gameobject);
				cnt++;
			}
			if (cnt >= max_count) {
				break;
			}
		}
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

	uint32_t Scene::Size(void) const noexcept {
		return m_gameObjects.size() - m_freelist.size();
	}
}
