#include "IComponent.hpp"
#include <string.h>
namespace MultiStation {

	IComponent::IComponent(const char* name, uint8_t len) noexcept {
		SetName(name, len);
	}
	IComponent::~IComponent(void) noexcept {

	}


	const char* IComponent::GetName(void) const noexcept {
		return m_name;
	}
	void IComponent::SetName(const char* name, uint8_t len) noexcept {
		strncpy_s(m_name, name, (len < 62) ? len : 62);
		m_name[63] = '\0';
	}

}
