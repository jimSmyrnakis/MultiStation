#pragma once
#include "mspch.h"
#include <Application.hpp>
namespace MultiStation{
	class ExampleSystem : public ISystem {
	public:
		ExampleSystem(void)  noexcept : ISystem("Examble System") {

		}
		virtual inline void OnAttach(void) noexcept {
			MS_INFO("Example System On Attach");
			Scene& ctx = Application::Get().GetScene();
			if (!ctx.HasRegister<int>()) {
				ctx.Register<int>();
			}
			float start = Time::GetTimeInSeconds();

			for (int i = 0; i < 10000; i++)
				ctx.CreateGameObject("test");
			ctx.UpdateScene();
			float end = Time::GetTimeInSeconds();
			MS_INFO("Time taken to create 10000 game object: %f seconds", end - start);
			start = Time::GetTimeInSeconds();
			std::vector<GameObject>& gameObjects = ctx.GetGameObjects();
			for (GameObject& obj : gameObjects) {
				obj.AddComponent<int>(0);
			}
			ctx.UpdateScene();
			end = Time::GetTimeInSeconds();
			MS_INFO("Time taken to create 10000 components: %f seconds", end - start);
			for (int i = 0; i < 5000; i++) {
				GameObject* obj = ctx.CreateGameObject("Test " + std::to_string(i));
				obj->AddComponent<int>(i);
			}
			ctx.CreateGameObject("Test");
			ctx.CreateGameObject("Player");
			ctx.CreateGameObject("Enemy");
		}

		virtual inline void OnDetach(void) noexcept {
			MS_INFO("Example System On Detach");
		}
		int hehehe = 0;
		virtual inline void OnUpdate(float deltaTime) noexcept {
			if (Input::IsKeyPressed(MS_KEY_T)) {
				MS_INFO("Tab key is pressed !");
			}
			/*JobSystem& refJob = Application::Get().GetJobSystem();
			std::shared_ptr<std::atomic<uint32_t>> counter = std::make_shared<std::atomic<uint32_t>>(0);
			refJob.ParallelFor([](struct Job job) {
				for (int i = 0;i < 10000; i++) {
					int j = i * i;
					int* heheptr = (int*)job.data;
					(*heheptr) += j;
				}
				}, &hehehe, 100, counter);
			refJob.WaitFor(counter);
*/
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
				MS_INFO("hehehe is %d", hehehe);
				for (int i =0 ; i < 100 ; i ++)
				Application::Get().GetScene().CreateGameObject("New Object");
				hehehe = 0;
				return true;
			}
			return false;
		}

	};
}
