#include "Core.hpp"

namespace MultiStation {

	

	void Application::Entry(Application* app) noexcept {
		MS_ENGINE_INFO("Hello World from SandBox3D!");
		app->SetName("SandBox3D");
		app->SetRunning(true);

		SystemManager* sysMgr = app->GetSystemManager();

		// Create all phases and bind systems to them
		uint32_t editorPhase = sysMgr->CreatePhase();
		sysMgr->BindPhase(editorPhase);
		sysMgr->AddSystem(std::make_shared<Editor>());


		// Run the systems
		while (app->IsRunning()) {
			sysMgr->ExecutePhase(editorPhase);
		}


	}
}
