#include "Core.hpp"

namespace MultiStation {

	class ExampleLayer : public Layer {

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

		}
		virtual inline void OnImGuiRender(void)noexcept {

		}

	};

	SystemManager* sysMgr = nullptr;
	uint32_t editorPhase = 0;
	void Application::OnStart(Application* app) noexcept {
		MS_ENGINE_INFO("Hello World from SandBox3D!");
		app->SetName("SandBox3D");
		app->SetRunning(true);
		app->PushLayer(new ExampleLayer());
		sysMgr = app->GetSystemManager();

		// Create all phases and bind systems to them
		editorPhase = sysMgr->CreatePhase();
		sysMgr->BindPhase(editorPhase);
		sysMgr->AddSystem(std::make_shared<Editor>());

		


	}

	void Application::OnUpdate(Application* app) noexcept {
		


		sysMgr->ExecutePhase(editorPhase);
		

	}


	void Application::OnLeave(Application* app) noexcept {

	}
}
