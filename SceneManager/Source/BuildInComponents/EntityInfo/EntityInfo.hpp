#pragma once
#include "../../Interfaces/IComponent.hpp"
#include <string>
namespace MultiStation {
	struct EntityInfo  : public IComponent{
		std::string name;
		
		EntityInfo(const std::string& name = "Entity") noexcept;

		inline EntityInfo(const EntityInfo& copy) : IComponent(copy) {
			name = copy.name;
		}
		inline EntityInfo(EntityInfo&& move) noexcept : IComponent(move){
			name = std::move(move.name);
		}

		inline EntityInfo& operator=(const EntityInfo& copy)  {
			name = copy.name;
			return *this;
		}
		inline EntityInfo& operator=(EntityInfo&& move) noexcept {
			name = std::move(move.name);
			return *this;
		}
	};
}
