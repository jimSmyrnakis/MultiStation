#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Components.hpp>
#include <ECS.hpp>
#include <vector>
#include <string>
/**
 * @author Dimitris Smyrnakis
 * @file GameObject.hpp
 * @class GameObject
 * @brief The GameObject class is the Front End of the ECS and it is that the end user 
 * should only use for Game Object's and nothing else . It Provides all the interface 
 * methods needed for handling Game Objects to the Game Engine . This Way the backend could change
 * for targeting more perfomance and better implementation's in the future 
 * where GameObject will remain the same .
 * @note Ofcourse in the future may have some additional methods 
 * but no matter what all previus versions should work without the need of the end user changing the code .  
 * @warning The GameObject is still under construction and is not stabilized to a final version 1.0.0 yet .
 * Fether more creating a game object on must not used derictly but it should be created by the scene class 
 * that will be provided by the engine ecosystem .
 * @version 1.0.0
 * @date 2026-03-03 (yy-mm-dd)
 */
namespace MultiStation {

	class GameObject {

	public:
		GameObject(ECSManager& context , uint32_t id) noexcept ;
		GameObject(const GameObject& other) noexcept  = delete;
		GameObject(GameObject&& other) noexcept ;
		~GameObject(void) noexcept ;

		GameObject& operator=(const GameObject& other) noexcept = delete ;
		GameObject& operator=(GameObject&& other) noexcept ;

	public: 
		/**  Interface  */

		/**
		 * @brief Adds a component to the Game Object . 
		 * @warning First only one component of the same type can be in a game object 
		 * . Second the component type T should always be register to the scene before 
		 * adding it to a game object .
		 * @tparam T The type of the component .
		 * @tparam Args Templated variadic arguments support .
		 * @param args The arguments needed for the component constructor .
		 * @return nothing
		 */
		template<typename T, typename... Args>
		void AddComponent(Args&&... args) noexcept ;

		/**
		 * @brief Checks if the Game Object has a component of type T .
		 * @tparam T The type of the component .
		 * @warning The component type T should always be register to the scene before calling this method .
		 * @returns true if the Game Object has a component of type T , false otherwise .
		 */
		template<typename T>
		bool HasComponent(void) const noexcept;

		/**
		 * @brief Removes the component of type T from the Game Object if exist's .
		 * @tparam T The type of the component .
		 * @warning The component type T should always be register to the scene before calling this method . 
		 * @returns Nothing .
		 */
		template<typename T>
		void RemoveComponent(void) noexcept;

		/**
		 * @brief Gets a pointer to the component of type T if exist's in the Game Object , nullptr otherwise .
		 * @tparam T The type of the component .
		 * @warning The component type T should always be register to the scene before calling this method .
		 */
		template<typename T>
		T* GetComponent(void) noexcept;

		/**
		 * @brief Gets a const pointer to the component of type T if exist's in the Game Object , nullptr otherwise .
		 * @tparam T The type of the component .
		 * @warning The component type T should always be register to the scene before calling this method .
		 */
		template<typename T>
		const T* GetComponent(void) const noexcept;

		/**
		 * @returns The name of the Game Object . Full length is 64 chars .
		 */
		const char* GetName(void) const noexcept;
/**
		 * @brief Sets the name of the Game Object .
		 * @param name The new name of the Game Object .
		 */
		void SetName(const char* name , uint8_t length) noexcept;

		/**
		 * @returns The names of all components of the Game Object .
		 * @brief The names always match the class name of the component type T , this 
		 * method can be usefull to the Editor or other systems that may need information . 
		 */
		std::vector<std::string> GetComponentsTypes(void) const noexcept;

		/**
		 * @brief Returns a unique id of this game objects
		 */
		uint32_t GetID(void) const noexcept;


	private:
		ECSManager* m_context = nullptr;
		uint32_t m_id;
		std::vector<std::string> m_componentTypes;
		char m_name[64];
	};



















	template<typename T, typename... Args>
	void GameObject::AddComponent(Args&&... args) noexcept {
		if (!m_context->HasRegister<T>()) {
			MS_ENGINE_WARN("Component type not registered. Skipping adding component.");
			return;
		}
		if (!HasComponent<T>()) {
			m_context->AddComponent<T>(m_id, args...);
			
		}
		else {
			MS_ENGINE_WARN("Game Object already has a component of this type. Skipping adding component.");
		}
	}


	template<typename T>
	bool GameObject::HasComponent(void) const noexcept {
		MS_ASSERT(m_context != nullptr, "GameObject context is null.");
		return m_context->HasComponent<T>(m_id);
	}


	template<typename T>
	void GameObject::RemoveComponent(void) noexcept {
		if (!m_context->HasRegister<T>()) {
			MS_ENGINE_WARN("Component type not registered. Skipping removing component.");
			return;
		}
		if (HasComponent<T>()) {
			m_context->RemoveComponent<T>(m_id);
		}
		else {
			MS_ENGINE_WARN("Game Object does not have a component of this type. Skipping removing component.");
		}
	}


	template<typename T>
	T* GameObject::GetComponent(void) noexcept {
		MS_ASSERT(m_context, "GameObject context is null.");
		return m_context->GetComponent<T>(m_id);
	}


	template<typename T>
	const T* GameObject::GetComponent(void) const noexcept {
		MS_ASSERT(m_context != nullptr, "GameObject context is null.");
		return m_context->GetComponent<T>(m_id);
	}














}
