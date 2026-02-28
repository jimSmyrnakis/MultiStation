#pragma once
#include "Core.hpp"
namespace MultiStation{
	class ExampleSystem : public ISystem {
	public:
		ExampleSystem(void)  noexcept : ISystem("Examble System") {

		}
		virtual inline void OnAttach(void) noexcept {

		}

		virtual inline void OnDetach(void) noexcept {

		}

		virtual inline void OnUpdate(float deltaTime) noexcept {
			if (Input::IsKeyPressed(MS_KEY_T)) {
				MS_INFO("Tab key is pressed !");
			}
		}
		virtual inline void OnEvent(Event& event)noexcept {
			MultiStation::EventDispatcher dispatcher(event);

			dispatcher.Dispatch<MultiStation::KeyPressedEvent>(
				BIND_EVENT_FN(ExampleSystem::OnKeyPressedEvent));
		}
		virtual inline void OnImGuiRender(float deltaTime) noexcept {
			ImGui::Begin("Text");
			ImGui::Text("Hello World !");
			ImGui::End();
		}

		inline bool OnKeyPressedEvent(MultiStation::KeyPressedEvent& e) {
			if (e.GetKeyCode() == MS_KEY_E) {
				MS_INFO("E key pressed :) ");
				return true;
			}
			return false;
		}

	};
}
