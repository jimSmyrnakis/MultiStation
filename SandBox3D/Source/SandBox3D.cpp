#include "Core.hpp"

namespace MultiStation {
	SystemManager* sysMgr = nullptr;
	uint32_t editorPhase = 0;
	void Application::OnStart(Application* app) noexcept {
		MS_ENGINE_INFO("Hello World from SandBox3D!");
		app->SetName("SandBox3D");
		app->SetRunning(true);

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
