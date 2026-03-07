#include "EntityInfo.hpp"

namespace MultiStation {

	EntityInfo::EntityInfo(const std::string& name ) noexcept : IComponent(name.c_str(), name.size()) {
		this->name = name;
	}

}
