#include "SceneManager.hpp"

namespace MultiStation {

	
	SceneManager::SceneManager(void) noexcept {
		m_registry = ComponentRegistry::Create();
		m_Systems = new std::vector<ISystem*>;
		ThreadPoolInit();
		
	}

	SceneManager::~SceneManager(void) noexcept {
		
		Release();
	}


	

	void SceneManager::Release(void) noexcept {
		
		CloseThreads();
		ThreadPoolFini();
		ComponentRegistry::Destroy(m_registry);
		m_registry = nullptr;
		m_Assingments.clear();
		if (m_Systems)
			delete m_Systems;
		m_Systems = nullptr;
		

		
	}
	SceneManager* SceneManager::Create(void) noexcept {
		return new SceneManager;
	}
	void SceneManager::Destroy(SceneManager* ptr) noexcept {
		if (ptr)
			delete ptr;
	}





	


}
