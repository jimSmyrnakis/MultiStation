#include "JobSystem.hpp"
#include <iostream>
#include <assert.h>
#include <functional>
#include <Platform.hpp>
namespace MultiStation {


	static thread_local uint32_t localWorkerID = BAD_ID; 

	// this variable is used to store the worker ID of the current thread for local job execution
	JobSystem::JobSystem(uint32_t workerCount) noexcept {
		m_workerCount = workerCount ? workerCount : std::thread::hardware_concurrency();
		if (m_workerCount == 0) m_workerCount = 1; // for vms cases

		m_gReadyQueues = new(std::nothrow) Queue<Job>[m_workerCount];
		MS_ASSERT(m_gReadyQueues, "Failed to allocate global ready queues");
		
		m_lReadyQueues = new(std::nothrow) Queue<Job>[m_workerCount];
		MS_ASSERT(m_lReadyQueues, "Failed to allocate local ready queues");
		
		m_workerThreads = new(std::nothrow) std::thread[m_workerCount -1];
		MS_ASSERT(m_workerThreads, "Failed to allocate worker threads");
		// we will use the main thread as a worker thread as well so we need one less thread than the worker count
		m_mainThreadID = std::this_thread::get_id();
		m_shutdown.store(false, std::memory_order_release);
		m_jobsInSystem.store(0, std::memory_order_release);
		m_QueueTurn.store(0, std::memory_order_relaxed);
		localWorkerID = m_workerCount - 1; // the main thread will have the last worker ID
		
		for (uint32_t i = 0; i < m_workerCount - 1; ++i) {
			m_workerThreads[i] = std::thread(&JobSystem::WorkerThread, this, i);
		}
		
	}

	JobSystem::~JobSystem(void) noexcept {
		Shutdown();
		delete[] m_gReadyQueues;
		delete[] m_lReadyQueues;
		delete[] m_workerThreads;
	}



	void JobSystem::AddJob(Job& job) noexcept {

		// dont allow adding jobs after shutdown is signaled
		if (m_shutdown.load(std::memory_order_acquire)) {
			MS_ENGINE_WARN("Cannot add job after shutdown is signaled");
			return;
		}

		// add one more job unit 
		m_jobsInSystem.fetch_add(1, std::memory_order_relaxed);

		// 
		if (job.counter) {
			job.counter->fetch_add(1, std::memory_order_relaxed);
		}

		

		// if the job is affine then add it to the local queue of the worker with the specified ID
		if (job.affine) {
			uint32_t affineWID = job.WorkerID;
			if (job.WorkerID >= m_workerCount) {
				affineWID = m_workerCount - 1;
				MS_ENGINE_WARN(
					"Affine job WorkerID %u is out of range, assigning to main thread WorkerID %u", 
					job.WorkerID, affineWID);

			}
			m_lReadyQueues[affineWID].Push(job);
		}
		else {
			
			uint32_t turn = m_QueueTurn.fetch_add(1, std::memory_order_relaxed);
			m_gReadyQueues[turn % m_workerCount].Push(job);
		}
		
	}

	void JobSystem::WaitFor(std::shared_ptr<std::atomic<uint32_t>>& counter) noexcept {
		bool localJob = false;
		
		while ((counter->load(std::memory_order_acquire) > 0)) {
			if (localWorkerID == BAD_ID) {
				std::this_thread::yield();
				continue;
			}
			Schedule(localWorkerID); // execute jobs in the main thread as well
		}
	}



	void JobSystem::WaitForAll(void) noexcept {
		bool localJob = false;
		while (m_jobsInSystem.load(std::memory_order_acquire) > 0) {
			// if the thread is the main thread 
			if (std::this_thread::get_id() == m_mainThreadID) {

				Schedule(m_workerCount - 1); // execute jobs in the main thread as well
			}
			else if (localWorkerID == BAD_ID)
			{
				std::this_thread::yield();
				continue;
			}
			else {
				MS_ASSERT(false, "WaitForAll should only be called from the main/parent thread of the scheduler or a non-worker thread");
			}
		}
	}


	void JobSystem::ParallelFor(
		JobFunction func, void* data,
		uint32_t jobCount, std::shared_ptr<std::atomic<uint32_t>> counter) noexcept {
		MS_ASSERT(func, "Job function cannot be null");
		MS_ASSERT(jobCount > 0, "Job count must be greater than zero");
		


		
		for (uint32_t i = 0; i < jobCount; ++i) {
			Job job;
			job.fun = func;
			job.data = data;
			job.affine = false;
			job.WorkerID = localWorkerID; // not used since isAffine is false , blockid is more usefull
			job.counter = counter;
			job.blockID = i;
			job.blockSize = jobCount;
			AddJob(job);
		}
	}

	void JobSystem::Shutdown(void) noexcept {
		if (m_shutdown.load(std::memory_order_acquire)) {
			MS_ENGINE_WARN("Shutdown already signaled , destructor calls it too");
			return;
		}
		m_shutdown.store(true, std::memory_order_release);

		if (std::this_thread::get_id() != m_mainThreadID) {
			MS_ASSERT(false, "Shutdown should only be called from the main/parent thread of the scheduler");
			return;
		}
		// Drain from main thread (critical για pinned-to-main jobs)
		if (std::this_thread::get_id() == m_mainThreadID) {
			while (m_jobsInSystem.load(std::memory_order_acquire) > 0) {
				Schedule(m_workerCount - 1);
			}
		}

		for (uint32_t i = 0; i < m_workerCount - 1; ++i) {
			if (m_workerThreads[i].joinable()) m_workerThreads[i].join();
		}
	}



















	


	void JobSystem::WorkerThread(uint32_t workerID) noexcept {
		localWorkerID = workerID;
		bool isRunning = true;
		while (isRunning ) {
			Schedule(workerID);
			isRunning =
				!m_shutdown.load(std::memory_order_acquire)
				|| (m_jobsInSystem.load(std::memory_order_acquire) > 0);
		}

	}

	void JobSystem::Schedule(uint32_t workerID) noexcept {
		static thread_local bool localJob = false;
		ExecuteJob(workerID, localJob);
		// alternate between local and global jobs
		localJob = !localJob;
	}

	void JobSystem::ExecuteJob(uint32_t workerID, bool local) noexcept {


		Job job;
		bool found;
		found = TryPopOrStealJob(workerID, job, local);
		if (!found) {
			// if no job found try to steal global job
			found = TryPopOrStealJob(workerID, job, !local);
		}

		if (found) {
			job.fun(job);
			if (job.counter  ) {
				
				job.counter->fetch_sub(1, std::memory_order_release);
			}

			m_jobsInSystem.fetch_sub(1, std::memory_order_release);
		}
		else
		{
			std::this_thread::yield();
		}


	}

	bool JobSystem::TryPopOrStealJob(uint32_t workerID, Job& job, bool local) noexcept {

		if (local) {
			if (m_lReadyQueues[workerID].Pop(job)) {
				return true;
			}
			return false;
		}

		if (m_gReadyQueues[workerID].Pop(job)) {
			return true;
		}

		// Try steal job from other worker global queue
		
		for (uint32_t i = 0; i < m_workerCount; ++i) {
			if (i == workerID)
				continue;
			if (m_gReadyQueues[i].Pop(job)) {
				return true;
			}
		}
		
		return false;
	}

}
