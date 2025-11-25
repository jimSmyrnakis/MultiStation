#include "SceneManager.hpp"

namespace MultiStation {
	void SceneManager::ThreadPoolInit(void) {
		unsigned int cores = std::thread::hardware_concurrency();
		uint32_t threads = ceil(2.5f * cores);
		m_Threads = new std::vector<std::thread>;
		// TODO : Print Info the amaunt of total threads available 
		m_threadsCount = threads;
		m_sharedBarrier = new std::barrier<>(m_threadsCount);
		m_sharedMutex = new std::shared_mutex;
		m_syncSharedBarrier = new std::barrier<>(floor(m_threadsCount * 0.8f)); // 80% of total threads is sync 
		// game loop threads
		// now initialize the assingments , Warning RunThreads if start no new system can be added
		// and assignment's must happen before starting the manager .
		m_Assingments.clear();
		for (int i = 0; i < m_threadsCount; i++) {
			m_Assingments[i]; // force to create empty vectors of system pointer's (inheritance reasons)
		}
		

	}

	

	void SceneManager::RunThreads(void) {
		m_ControlBlock.SetState(ManagerState::UPDATE);
		std::vector<ManagerInfo*>* InfoList = new std::vector<ManagerInfo*>;
		
		m_Threads->clear();
		for (int i = 0; i < floor(m_threadsCount * 0.8f); i++) {
			ManagerInfo* syncInfo = new ManagerInfo;
			
			syncInfo->registry = m_registry;
			syncInfo->sharedBarrier = m_sharedBarrier;
			syncInfo->syncSharedBarrier = m_syncSharedBarrier;
			syncInfo->synchronized = true;
			syncInfo->sharedLock = m_sharedMutex;
			syncInfo->systems = &m_Assingments[i];
			syncInfo->controlBlock = &m_ControlBlock;
			m_Threads->emplace_back(systems_manager, syncInfo);
			InfoList->push_back(syncInfo);
		}
		
		for (int i = floor(m_threadsCount * 0.8f); i < (m_threadsCount-1); i++) {
			ManagerInfo* asyncInfo = new ManagerInfo;

			asyncInfo->registry = m_registry;
			asyncInfo->sharedBarrier = m_sharedBarrier;
			asyncInfo->syncSharedBarrier = m_syncSharedBarrier;
			asyncInfo->synchronized = false;
			asyncInfo->sharedLock = m_sharedMutex;
			asyncInfo->systems = &m_Assingments[i];
			asyncInfo->controlBlock = &m_ControlBlock;
			m_Threads->emplace_back(systems_manager, asyncInfo);
			InfoList->push_back(asyncInfo);
		}

		// call this thread as async
		ManagerInfo* asyncInfo = new ManagerInfo;

		asyncInfo->registry = m_registry;
		asyncInfo->sharedBarrier = m_sharedBarrier;
		asyncInfo->synchronized = false;
		asyncInfo->sharedLock = m_sharedMutex;
		asyncInfo->systems = &m_Assingments[m_threadsCount - 1];
		asyncInfo->controlBlock = &m_ControlBlock;
		asyncInfo->syncSharedBarrier = m_syncSharedBarrier;
		InfoList->push_back(asyncInfo);
		systems_manager(asyncInfo);

		// join workers
		for (auto& t : *m_Threads) {
			if (t.joinable()) t.join();
		}
		m_Threads->clear();

		// once the main thread leave from there destroy managers info
		for (ManagerInfo*& info : *InfoList) {
			delete info;
			info = nullptr;
		}
		delete InfoList;


	}

	void SceneManager::CloseThreads(void) {
		m_ControlBlock.SetState(ManagerState::DESTROY);
		// the destroy state says in all threads that they will
		// need to run one last loop with destroy methods called
		// and before leaving they will made sure that they will
		// sync first via shared barrier

	}

	void SceneManager::ThreadPoolFini(void) {
		m_threadsCount = 0;
		if (m_Threads)
			delete m_Threads;
		m_Threads = nullptr;

		if (m_sharedMutex)
			delete m_sharedMutex;
		m_sharedMutex = nullptr;


		if (m_sharedBarrier)
			delete m_sharedBarrier;
		m_sharedBarrier = nullptr;

		if (m_syncSharedBarrier)
			delete m_syncSharedBarrier;
		m_syncSharedBarrier = nullptr;

		if (m_Systems)
			delete m_Systems;
		m_Systems = nullptr;

		m_Assingments.clear();
	}
}
