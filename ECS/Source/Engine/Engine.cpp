#include "Engine.hpp"

namespace MultiStation {

	Engine::Engine(void) : m_serializationRegistry() , m_sceneManager(m_context , m_serializationRegistry) , m_systemManager(m_context) {

	}

	void Engine::OnUpdate(float dt) {
		m_systemManager.OnUpdate(dt);
	}


	void Engine::OnEvent(Event& e) {
		m_systemManager.OnEvent(e);
	}

}
