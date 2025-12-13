#pragma once
#include "../../TaskScheduller.hpp"
#include <vector>
#include <unordered_map>
#include <shared_mutex>
#include <barrier>
namespace MultiStation
{

	struct IPipelineTaskMetadata : public ITaskMetadata
	{
		virtual ~IPipelineTaskMetadata(void) noexcept = default;
		uint16_t stage = 0;
		bool canRunInParallel = false;
		
	};


	class Pipeline : public TaskScheduler
	{

		struct Stage
		{
			// threadid -> tasks
			std::unordered_map<uint16_t, std::vector<Task*>> parallelTasks;
			std::unordered_map<uint16_t, std::vector<IPipelineTaskMetadata*>> parallelMetadatas;
			uint16_t m_currentThread = 0; // for scheduling non parallel tasks uniformly among threads
		};
	public:
		~Pipeline(void) noexcept override = default;
		Pipeline(void) noexcept = default;

		// Called only from one thread
		void Initialize(uint16_t threads) override;
		// Called only from one thread
		void Finalize(void) override;
		// Called from multiple threads
		void RegisterTask(Task* task, ITaskMetadata* metadata) override;
		// Called from multiple threads
		void UnregisterTask(Task* task) override;
		// Called from multiple threads
		void Schedule(uint32_t threadID) override;
		// Called from a thread to specify that is leaving
		void NotifyThreadExit(uint32_t threadID) override;

		Pipeline(Pipeline&&) = delete;
		Pipeline& operator=(Pipeline&&) = delete;
		Pipeline(const Pipeline&) = delete;
		Pipeline& operator=(const Pipeline&) = delete;

	private:
		uint16_t m_threadCount = 0;
		std::shared_mutex m_mutex;
		std::barrier<>* m_barrier = nullptr;
		uint32_t m_totalStages = 0;
		std::vector<Stage> m_stages;
		uint32_t m_currentStage = 0;

	};
}
