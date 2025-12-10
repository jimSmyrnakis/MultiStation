#pragma once
#include "TaskScheduller.hpp"
#include <thread>
#include <barrier>
namespace MultiStation {

	class TaskManager {

	public:

		TaskManager(TaskScheduler* scheduler , uint16_t threads , bool includeMainThread = true) noexcept ;
		~TaskManager(void) noexcept;
		TaskManager(const TaskManager&) = delete;
		TaskManager& operator=(const TaskManager&) = delete;
		TaskManager(TaskManager&& other) noexcept;
		TaskManager& operator=(TaskManager&& other) noexcept;

		
		void AddTask(Task* task);

		void RemoveTask(Task* task);

		void Start(void);
		void Stop(void);



	private:
		TaskScheduler* m_Scheduler = nullptr;
		uint16_t m_ThreadCount = 1;
		bool m_includeMainThread = false;
		std::vector<Task*> m_Tasks;
		std::vector<std::thread> m_Threads;
		std::barrier<>* m_Barrier = nullptr;
	};

}
