#include "WindowProperties.hpp"

namespace MultiStation {

	WindowProperties::WindowProperties(
		uint32_t Width,
		uint32_t Height,
		std::string Title,
		bool IsVSync) noexcept {

		this->Height = Height;
		this->Width = Width;
		this->Title = Title;
		this->IsVSync = IsVSync;
	}

}
