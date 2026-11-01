#include "Engine.hpp"

namespace MultiStation {

	Engine::Engine(void) {
		
		m_context.GetSerializer = [this]() -> SerializationRegistry* {
			return &m_sregistry;
		};

		m_sceneManager = new SceneManager(m_context , m_objectMap , m_sregistry);

		m_systemManager = new SystemManager(m_context);

		
		
	}

	void Engine::OnUpdate(float dt) {
		
		// update systems
		m_systemManager->OnUpdate(dt);

		// update scene
		m_sceneManager->ProcessPendingCommands();
	}


	void Engine::OnEvent(Event& e) {
		//m_systemManager->OnEvent(e);
	}

	Engine::~Engine(void) {
		delete m_systemManager;
		delete m_sceneManager;
	}


	void Engine::Init(void) noexcept {
		m_systemManager->InitModules();
		m_sceneManager->Init();
	}

	void Engine::Fini(void) noexcept{
		m_sceneManager->Fini();
		m_systemManager->FiniModules();
	}

}
