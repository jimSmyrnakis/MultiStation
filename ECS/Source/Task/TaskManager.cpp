#include "TaskManager.hpp"

namespace MultiStation {
	void* ManagerWorker(void* managerInfo) {
		// Cast the managerInfo to ManagerInfo struct
		TaskManager::ManagerInfo* info = static_cast<TaskManager::ManagerInfo*>(managerInfo);
		if (!info || !info->scheduler) {
			return nullptr; // Invalid info
			// TODO : Fatal Error 
		}

		// Get thread-specific data
		uint16_t threadID = info->threadID;
		TaskScheduler* scheduler = info->scheduler;
		uint16_t totalThreads = info->totalThreads;
		std::atomic<TaskManager::State>* state = info->state;
		// Main loop
		
		while (true) {
			// protect state read
			if (state->load(std::memory_order_acquire) == TaskManager::Stopped) {
				scheduler->NotifyThreadExit(threadID);
				break;
			}
			scheduler->Schedule(threadID);
			
		}
		
		// Clean up info , for moment is here
		if (info) {
			delete info;
			info = nullptr;
		}
		

		

		return nullptr;
	}

	TaskManager::TaskManager(
		TaskScheduler* scheduler,
		uint16_t threads) noexcept {

		m_Scheduler = scheduler;
		m_ThreadCount = (threads == 0) ? 1 : threads;
		m_state = Stopped;
		
		
	}

	TaskManager::~TaskManager(void) noexcept {
		if (this->GetState() == Running)
		{
			Stop();
			JoinAll();
		}
	}

	void TaskManager::AddTask(Task* task, ITaskMetadata* metadata)
	{
		std::lock_guard lock(m_mutex);
		if (m_state.load() == Running) {
			m_Scheduler->RegisterTask(task, metadata);
		}
		else {
			m_pendingTasks.push_back({ task, metadata });
		}
	}

	void TaskManager::RemoveTask(Task* task)
	{
		std::lock_guard lock(m_mutex);
		if (m_state.load() == Running) {
			m_Scheduler->UnregisterTask(task);
		}
		else {
			std::erase_if(m_pendingTasks, [&](auto& p) { return p.task == task; });
		}
	}

	// Start the task manager and its threads
	void TaskManager::Start(bool mainAsWorker) {
		if (m_state.load(std::memory_order_acquire) == Running)
			return;

		// Initialize the scheduler
		m_Scheduler->Initialize(m_ThreadCount);
		bool expected = false;
		m_finalized.exchange(false);

		// Register pending tasks now that scheduler is initialized
		for (auto& p : m_pendingTasks)
			m_Scheduler->RegisterTask(p.task, p.metadata);

		// Set state to running
		m_state.store(Running, std::memory_order_release);
		

		uint32_t threads =  m_ThreadCount ;
		uint16_t i = (mainAsWorker) ? 1 : 0;
		// Create and start threads
		m_Threads.reserve(threads);
		for (i = i; i < threads; i++) {
			TaskManager::ManagerInfo* info = new TaskManager::ManagerInfo();
			info->scheduler = m_Scheduler;
			info->threadID = i;
			info->totalThreads = m_ThreadCount;
			info->state = &m_state;

			m_Threads.emplace_back(ManagerWorker, static_cast<void*>(info));
		}
		
		if (!mainAsWorker)
			return;
		// Main thread also works as a worker
		TaskManager::ManagerInfo* info = new TaskManager::ManagerInfo();
		info->scheduler = m_Scheduler;
		info->threadID = m_ThreadCount -1;
		info->totalThreads = m_ThreadCount;
		info->state = &m_state;
		//m_Threads.emplace_back(std::this_thread::get_id());
		
		ManagerWorker(static_cast<void*>(info));
		// we are stopping here 
		
	}

	// Stop the task manager and its threads
	void TaskManager::Stop(void) {
		if (m_state.load(std::memory_order_acquire) == Stopped)
			return;
		// Set state to stopped
		m_state.store(Stopped, std::memory_order_release);

	}

	void TaskManager::JoinAll(void) {
		std::thread::id current = std::this_thread::get_id();

		for (auto& t : m_Threads) {
			if (t.get_id() == current) continue;
			if (t.joinable()) t.join();
		}

		m_Threads.clear();

		bool expected = false;
		if (m_finalized.compare_exchange_strong(expected, true)) {
			if (m_Scheduler) m_Scheduler->Finalize();
		}
	}

	TaskManager::State TaskManager::GetState(void) const {
		return m_state.load(std::memory_order_acquire);
	}

	uint32_t TaskManager::GetThreads(void) const {
		return m_ThreadCount;
	}

}
