#pragma once
#include <Media.hpp>

namespace MultiStation {

	class ImGuiLayer : public Layer {

	public:
		ImGuiLayer(void) noexcept;
		~ImGuiLayer(void) noexcept;

		ImGuiLayer(const ImGuiLayer& other) = delete;
		ImGuiLayer(ImGuiLayer&& other) = delete;

		ImGuiLayer& operator=(const ImGuiLayer& other) = delete;
		ImGuiLayer& operator=(ImGuiLayer&& other) = delete;

	public:


		virtual void OnAttach(void) noexcept override ;


		void Begin(void) noexcept;
		
		void OnUIRender(float dt) noexcept override;

		void End(void) noexcept;

		
		virtual void OnDetach(void) noexcept override;

	};

}
