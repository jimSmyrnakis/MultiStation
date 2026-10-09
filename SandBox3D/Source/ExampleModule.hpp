#pragma once
#include "mspch.h"
#include <Application.hpp>
namespace MultiStation{

	class ExampleSystem : public ISystem {
	public:

		virtual inline void OnAttach(void) noexcept override {
			MS_INFO("Example System On Attach");
		}

		virtual inline void OnDettach(void) noexcept override {
			MS_INFO("Example System On Detach");
		}

		virtual inline void OnUpdate(float deltaTime) noexcept override {
			if (Input::IsKeyPressed(MS_KEY_T)) {
				MS_INFO("Tab key is pressed !");
			}
		}

		virtual inline void OnEvent(Event& event) noexcept override {
			MultiStation::EventDispatcher dispatcher(event);
			dispatcher.Dispatch<MultiStation::KeyPressedEvent>(
				BIND_EVENT_FN(ExampleSystem::OnKeyPressedEvent));
		}

		inline bool OnKeyPressedEvent(MultiStation::KeyPressedEvent& e) {
			if (e.GetKeyCode() == MS_KEY_W) {
				MS_INFO("W key pressed :) ");
				
				return true;
			}
			return false;
		}
	};

	class ExampleModule : public ISystemModule {
	public:
		ExampleModule(void)  noexcept  {

		}

		virtual inline void Init(EngineContext& ctx) noexcept override {
			MS_INFO("Example Module Initialized");
			
		}

		virtual inline void Fini(EngineContext& ctx) noexcept override {
			MS_INFO("Example Module Finilized");
		}

		virtual inline ISystem& GetSystem(void) noexcept override {
			return m_system;
		}
		
		
	private:
		ExampleSystem m_system;
		

	};
}
