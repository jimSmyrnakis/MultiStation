#pragma once
#include "../GameObject/GameObject.hpp"

/**
 * @file Scene.hpp
 * @class Scene
 * @brief The Scene class is what the end user will use to create and manage game objects in the game engine.
 * @details The Scene class provides a way the end user to create / remove game objects , register components 
 * or to get all components of the same type in the scene . This class must work the same in all versions 
 * so the end user code will not be affected by any change in the backend of the engine .
 * @warning The end user should not create a scene class directly but it will provided by the engine ecosystem .
 * @version 1.0.0
 * @date 2026-03-04 (yy-mm-dd)
 */
namespace MultiStation {

	class Scene {

	public:
		Scene(ECSManager& context) noexcept;
		Scene(const Scene& other) noexcept = delete;
		Scene(Scene&& other) noexcept ;
		~Scene(void) noexcept;

		Scene& operator=(const Scene& other) noexcept = delete;
		Scene& operator=(Scene&& other) noexcept;

	public:

		/**
		 * @brief Creates a new Game Object in the scene with the specified name. If no name is provided, a default name "GameObject" is used.
		 * @returns A pointer to the newly created Game Object. 
		 * @param name The name of the Game Object to be created. If not provided, it defaults to "GameObject".
		 * 
		 */
		GameObject* CreateGameObject(const std::string& name = "GameObject") noexcept;
		
		/**
		 * 
		 * 
		 * @param gameObject Game Object to be removed from the scene. 
		 * @brief Removes game object from the scene . If the Game Object is not 
		 * found in the scene, a warning message is generated and no action is taken.
		 *  
		 */
		void RemoveGameObject(GameObject* gameObject) noexcept;

		/**
		 * @returns A reference to the vector containing all Game Objects in the scene.
		 */
		std::vector<GameObject>& GetGameObjects(void) noexcept;

		/**
		 * 
		 * @returns A const reference to the vector containing all Game Objects in the scene.
		 */
		const std::vector<GameObject>& GetGameObjects(void) const noexcept;

		/**
		 * @returns A pointer to the Game Object with the specified ID if found in the scene, nullptr otherwise.
		 * 
		 * \param id The unique identifier of the Game Object .
		 */
		GameObject* GetGameObjectByID(uint32_t id) noexcept;

		/**
		 * @returns A pointer to the Game Object with the specified ID if found in the scene, nullptr otherwise.
		 *
		 * \param id The unique identifier of the Game Object .
		 */
		const GameObject* GetGameObjectByID(uint32_t id)const noexcept;

		/**
		 * @brief Registers a new component type T to the scene. 
		 * This allows adding/removing/replacing components of this type to game objects and other functionalities.
		 */
		template<typename T>
		void Register(void) noexcept;

		/**
		 * @returns True if the component type T is already registered in the scene, false otherwise.
		 */
		template<typename T>
		bool HasRegister(void) const noexcept;


		/**
		 * 
		 * @returns A reference to the vector containing all components of type T in the scene.
		 */
		template<typename T> 
		std::vector<T>& GetComponents(void) noexcept;

		/**
		 * @returns A const reference to the vector containing all components of type T in the scene.
		 */
		template<typename T>
		const std::vector<T>& GetComponents(void) const noexcept;

		/**
		 * @brief Gets the name of the scene.
		 *
		 */
		std::string GetSceneName(void) const noexcept;

		/**
		 * @brief Changes the name of the scene to the specified name.
		 * 
		 * @param name new scene name
		 * 
		 */
		void SetSceneName(const std::string& name) noexcept;
		
		/**
		 * @returns A vector containing the names of all registered component types in the scene. The names always match the class name of the component type T.
		 * 
		 * 
		 * \return 
		 */
		std::vector<std::string> GetRegisteredComponentTypes(void) const noexcept;

		/**
		 * @brief Updates the scene by executing all pending operations to the backend , 
		 * @warning Do not use this method inside a system , this method should be call at a pecific 
		 * stages in the game engine pipeline . 
		 * 
		 */
		void UpdateScene(void) noexcept;

	private:
		ECSManager& m_context;
		std::string m_sceneName;
		std::vector<GameObject> m_gameObjects;
		std::unordered_map<uint32_t, size_t> m_gameObjectIDToIndex;
		std::vector<std::string> m_registeredComponentTypes;
		uint32_t m_nextGameObjectID;
	};


	template<typename T>
	void Scene::Register(void) noexcept {
		if (m_context.HasRegister<T>()) {
			MS_ENGINE_WARN("Component type already registered in the scene. Skipping registration.");
			return;
		}
		m_context.Register<T>();
		m_registeredComponentTypes.push_back(typeid(T).name());
	}

	template<typename T>
	bool Scene::HasRegister(void) const noexcept {
		return m_context.HasRegister<T>();
	}


	template<typename T>
	std::vector<T>& Scene::GetComponents(void) noexcept {
		return m_context.GetComponents<T>();
	}

	template<typename T>
	const std::vector<T>& Scene::GetComponents(void) const noexcept {
		return m_context.GetComponents<T>();
	}

}
