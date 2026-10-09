#include "Engine.hpp"

namespace MultiStation {

	Engine::Engine(void) : m_serializationRegistry() {
		m_context.GetSerializer = [this]() -> SerializationRegistry& {
			return m_serializationRegistry;
		};
		m_sceneManager = new SceneManager(m_context, m_serializationRegistry);

		m_systemManager = new SystemManager(m_context);
		
	}

	void Engine::OnUpdate(float dt) {
		m_systemManager->OnUpdate(dt);
	}


	void Engine::OnEvent(Event& e) {
		m_systemManager->OnEvent(e);
	}

	Engine::~Engine(void) {
		delete m_systemManager;
		delete m_sceneManager;
	}

}
