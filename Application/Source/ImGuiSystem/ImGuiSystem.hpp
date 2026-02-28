#pragma once
#include <ECS.hpp>

namespace MultiStation {

	class ImGuiSystem : public ISystem {

	public:
		ImGuiSystem(void) noexcept;
		~ImGuiSystem(void) noexcept;

		ImGuiSystem(const ImGuiSystem& other) = delete;
		ImGuiSystem(ImGuiSystem&& other) = delete;

		ImGuiSystem& operator=(const ImGuiSystem& other) = delete;
		ImGuiSystem& operator=(ImGuiSystem&& other) = delete;

	public:


		virtual void OnAttach(void) noexcept override ;


		void Begin(void) noexcept;
		
		virtual void OnImGuiRender(float deltaTime) noexcept override;

		void End(void) noexcept;

		
		virtual void OnDetach(void) noexcept override;

	};

}
