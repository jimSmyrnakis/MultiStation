#pragma once
#include "../../TaskScheduller.hpp"
#include "../../../Utilities/Queue/Queue.hpp"
#include "../../../Utilities/DAG/DAGraph.hpp"
#include <vector>
#include <stdint.h>
#include <stddef.h>
#include <shared_mutex>
#include <barrier>

namespace MultiStation{

	struct DAGSchedulerTaskMetadata {
		
	};

	class DAGScheduler : public TaskScheduler {

	public:
		~DAGScheduler(void) noexcept = default;

		// Called only from one thread
		virtual void Initialize(uint16_t threads) ;

		// Called only from one thread
		virtual void Finalize(void) ;



		// Called from multiple threads
		virtual void RegisterTask(Task* task, ITaskMetadata* metadata) ;

		// Called from multiple threads
		virtual void UnregisterTask(Task* task) ;

		// Called from multiple threads
		virtual void Schedule(uint32_t threadID);

		// Notify that a thread is exiting , use for sync notification
		// like barrier arrive and drop instead of arrive and wait
		virtual void NotifyThreadExit(uint32_t threadID) ;

	private:
		DAGraph m_graph;
		std::barrier<>* m_sync;
		uint32_t m_threads;
		Queue<uint32_t> m_readyQue;
		std::vector<Task*> m_tasks;
		std::vector<ITaskMetadata*> m_metadata;

	};

}
