#pragma once
#include <stdint.h>
#include <stddef.h>
namespace MultiStation{

	class IComponent {

	public:
		IComponent(const char* name, uint8_t len) noexcept;
		virtual ~IComponent(void) noexcept;


		const char* GetName(void) const noexcept;
		void SetName(const char* name, uint8_t len) noexcept;

	private:
		char m_name[64];
	};

}
