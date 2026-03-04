#pragma once
#include <ECS.hpp>
#include <string>
namespace MultiStation {
	struct EntityInfo  {
		std::string name;
		
		EntityInfo(const std::string& name = "Entity") noexcept;

		inline EntityInfo(const EntityInfo& copy) {
			name = copy.name;
		}
		inline EntityInfo(EntityInfo&& move) noexcept {
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
