#include "SceneManager.hpp"

namespace MultiStation {

	uint32_t SceneManager::GetSyncThreadsNum(void) const {
		return floor(m_threadsCount * 0.8f);
	}
	uint32_t SceneManager::GetAsyncThreadsNum(void) const {
		return m_threadsCount - floor(m_threadsCount * 0.8f);
	}

	void SceneManager::AssignSyncSystem(uint32_t syncThreadIndex, ISystem* system) {
		if (syncThreadIndex >= GetSyncThreadsNum()) {
			// TODO : Fatal Error Log
			return;
		}
		uint32_t indexThread = syncThreadIndex;
		m_Assingments[indexThread].push_back(system);
		m_Systems->push_back(system);
	}
	void SceneManager::AssignAsyncSystem(uint32_t asyncThreadIndex, ISystem* system) {
		if (asyncThreadIndex >= GetAsyncThreadsNum()) {
			// TODO : Fatal Error Log
			return;
		}
		uint32_t indexThread = GetSyncThreadsNum() + asyncThreadIndex;
		m_Assingments[indexThread].push_back(system);
		m_Systems->push_back(system);
	}

	void SceneManager::StartSystems(void) {
		// For moment only once this is set up
		this->RunThreads();
	}
	// i now no good check if these recalled , this may happen later
	// for moment i want to see it run first 
	void SceneManager::StopSystems(void) {
		this->CloseThreads();
		this->ThreadPoolFini();
	}

}
