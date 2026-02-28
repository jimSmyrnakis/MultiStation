#pragma once
#include "../Core.hpp"
namespace MultiStation {
	class Editor : public ISystem {
	public:
		Editor(void) noexcept;
		~Editor(void) noexcept;
		Editor(const Editor&) noexcept = delete;
		Editor& operator=(const Editor&) noexcept = delete;
		Editor(Editor&&) noexcept = delete;
		Editor& operator=(Editor&&) noexcept = delete;

		virtual void OnUpdate(float deltaTime) noexcept override;
	};
}	
