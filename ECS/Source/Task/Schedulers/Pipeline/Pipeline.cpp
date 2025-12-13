#include "Pipeline.hpp"

namespace MultiStation {

	// Called only from one thread
	void Pipeline::Initialize(uint16_t threads) {
		m_threadCount = threads;
		m_barrier = new std::barrier<>(m_threadCount);
		// TODO : Error if threads == 0 or threads > MAX_THREADS or m_barrier == nullptr
		m_currentStage = 0;
		m_totalStages = 1;
		// For the first stage do initialization for each thread
		Stage& stage = m_stages.emplace_back();
		stage.m_currentThread = 0;
		for (uint16_t i = 0; i < m_threadCount; i++) {
			stage.parallelTasks[i] = std::vector<Task*>();
			stage.parallelMetadatas[i] = std::vector<IPipelineTaskMetadata*>();

		}

	}
	// Called only from one thread
	void Pipeline::Finalize(void) {
		if (m_barrier) {
			delete m_barrier;
			m_barrier = nullptr;
		}

		// delete all tasks from all stages
		m_stages.clear();
	}
	// Called from multiple threads
	void Pipeline::RegisterTask(Task* task, ITaskMetadata* metadata) {
		std::unique_lock<std::shared_mutex> lock(m_mutex);
		// exception safety too

		IPipelineTaskMetadata* pipelineMetadata = dynamic_cast<IPipelineTaskMetadata*>(metadata);
		if (!pipelineMetadata) {
			return; // TODO : Error handling
		}

		// create that many stages if not exist
		while (pipelineMetadata->stage >= m_stages.size()) {
			Stage& stage = m_stages.emplace_back();
			m_totalStages++;
			// Initialize them per thread
			stage.m_currentThread = 0;
			for (uint16_t i = 0; i < m_threadCount; i++) {
				stage.parallelTasks[i] = std::vector<Task*>();
				stage.parallelMetadatas[i] = std::vector<IPipelineTaskMetadata*>();
			}
		}
		
		// Get Stage info
		Stage& stage = m_stages[pipelineMetadata->stage];

		// Add the new task to the stage
		if (pipelineMetadata->canRunInParallel) { // here add the task to all threads
			for (uint16_t i = 0; i < m_threadCount; i++) {
				stage.parallelTasks[i].push_back(task);
				stage.parallelMetadatas[i].push_back(pipelineMetadata);
			}
			stage.m_currentThread = 0; // for next task to be added to thread 0
			
		}
		else {
			// Add the task to the current thread
			uint16_t threadID = stage.m_currentThread;
			stage.parallelTasks[threadID].push_back(task);
			stage.parallelMetadatas[threadID].push_back(pipelineMetadata);
			// Update the current thread for next non parallel task
			stage.m_currentThread = (stage.m_currentThread + 1) % m_threadCount;
		}
	}
	// Called from multiple threads
	void Pipeline::UnregisterTask(Task* task)  {
		std::unique_lock<std::shared_mutex> lock(m_mutex);
		

		// find and remove the task from all stages
		for (Stage& stage : m_stages) {
			for (auto& [threadID, tasks] : stage.parallelTasks) {
				auto it = std::find(tasks.begin(), tasks.end(), task);
				if (it != tasks.end()) {
					size_t index = std::distance(tasks.begin(), it);
					auto itMeta = stage.parallelMetadatas[threadID].begin() + index;
					stage.parallelMetadatas[threadID].erase(itMeta);
					tasks.erase(it);
				}
			}
		}

		// May not the best way to do it but it works
		// and is for maynly registering during initialization phase
		// for the Game engine

	}
	// Called from multiple threads
	void Pipeline::Schedule(uint32_t threadID)  {
		
		std::shared_lock slokc(m_mutex);
		m_mutex.lock_shared();
		uint32_t currentStage = m_currentStage;

		Stage& stage = m_stages[currentStage];

		// Get copy of tasks for this thread
		auto tasks = stage.parallelTasks[threadID];
		slokc.unlock();
		// Run all tasks for this thread
		for (Task* task : tasks) {
			if (task) {
				task->Run(threadID);
			}
		}
		// sync here
		m_barrier->arrive_and_wait();
		
		// master thread update the current stage for next schedule
		if (threadID == 0)
		{
			m_currentStage = (m_currentStage + 1) % m_totalStages;
		}
		
		// sync here
		m_barrier->arrive_and_wait();
	}

	void Pipeline::NotifyThreadExit(uint32_t threadID) {
		m_barrier->arrive_and_drop();
	}

}
