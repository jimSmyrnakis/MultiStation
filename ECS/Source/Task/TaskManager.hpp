#pragma once
#include "TaskScheduller.hpp"
#include <thread>
#include <barrier>
#include <shared_mutex>
namespace MultiStation {
	void* ManagerWorker(void* managerInfo);
	class TaskManager {
	public:
		enum State {
			Stopped,
			Running
		};

		struct ManagerInfo {
			TaskScheduler* scheduler = nullptr;
			uint16_t threadID = 0;
			uint16_t totalThreads = 1;
			std::atomic<State>* state = nullptr;
			std::barrier<>* barrier = nullptr;
		};

		struct PendingTask {
			Task* task = nullptr;
			ITaskMetadata* metadata = nullptr;
		};
	public:

		TaskManager(TaskScheduler* scheduler , uint16_t threads ) noexcept ;
		~TaskManager(void) noexcept;
		TaskManager(const TaskManager&) = delete;
		TaskManager& operator=(const TaskManager&) = delete;
		TaskManager(TaskManager&& other) noexcept = delete;
		TaskManager& operator=(TaskManager&& other) noexcept = delete;

		
		// Thread safe
		void AddTask(Task* task , ITaskMetadata* metadata);

		// Thread safe
		void RemoveTask(Task* task);

		// Start the task manager and its threads
		void Start(bool mainAsWorker);

		// Stop the task manager and its threads , but not join them
		void Stop(void);

		// Use only from the main thread , and use it always
		// after the start , if the main thread calls stop then use 
		// it there too
		void JoinAll(void);

		// Get State (Running or stopped ) of the task manager
		State GetState(void) const;

		uint32_t GetThreads(void) const;

	private:
		TaskScheduler*				m_Scheduler = nullptr;
		uint16_t					m_ThreadCount = 1;
		std::vector<PendingTask>	m_pendingTasks;
		std::vector<std::thread>	m_Threads;
		mutable std::atomic<State>	m_state = Stopped;
		std::atomic<bool>			m_finalized{ false };
		std::mutex					m_mutex;
	};

}
