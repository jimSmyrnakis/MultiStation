#include "GameObject.hpp"

#include <string.h>
#include <memory.h>
namespace MultiStation {

	GameObject::GameObject(ECSManager& context, uint32_t id) noexcept {
		this->m_context = &context;
		this->m_id = id;
		MS_ASSERT(m_name, "No memory!!!");
		memset(m_name, 0, sizeof(char) * 64);
		strncpy(m_name, "GameObject", 11);
	}
	
	GameObject::GameObject(GameObject&& other) noexcept {
		this->m_context = other.m_context;
		this->m_id = other.m_id;
		
		strncpy(this->m_name , other.m_name , 63);

		other.m_context = nullptr;
		other.m_id = 0;
	
	}
	GameObject::~GameObject(void) noexcept {
		
	}

	

	GameObject& GameObject::operator=(GameObject&& other) noexcept {
		if (this == &other) return *this;
		this->m_context = other.m_context;
		this->m_id = other.m_id;

		strncpy(this->m_name, other.m_name, 63);

		other.m_context = nullptr;
		other.m_id = 0;
		return *this;
	}


	
	const char* GameObject::GetName(void) const noexcept {
		MS_ASSERT(m_context != nullptr, "GameObject context is null.");
		return m_name;
	}
	/**
			 * @brief Sets the name of the Game Object .
			 * @param name The new name of the Game Object .
			 */
	void GameObject::SetName(const char* name, uint8_t length) noexcept {
		MS_ASSERT(m_context != nullptr, "GameObject context is null.");
		strncpy(m_name, name, std::min(length, (uint8_t)63) );
	}


	std::vector<std::string> GameObject::GetComponentsTypes(void) const noexcept {
		return m_componentTypes;
	}

	uint32_t GameObject::GetID(void) const noexcept{
		return m_id;
	}


}
