#pragma once

#include "Task.hpp"
#include "ITaskMetadata.hpp"
namespace MultiStation {

	class TaskScheduler {
	public:
		virtual ~TaskScheduler(void) noexcept = default;
		
		// Called only from one thread
		virtual void Initialize(uint16_t threads) = 0;

		// Called only from one thread
		virtual void Finalize(void) = 0;



		// Called from multiple threads
		virtual void RegisterTask(Task* task , ITaskMetadata* metadata) = 0;
		
		// Called from multiple threads
		virtual void UnregisterTask(Task* task) = 0;
		
		// Called from multiple threads
		virtual void Schedule(uint32_t threadID) = 0;
	};

}
