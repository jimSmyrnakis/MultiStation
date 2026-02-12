#include "JobSystem.hpp"
#include <iostream>
#include <assert.h>
namespace MultiStation {


	static thread_local uint32_t localWorkerID = BAD_ID; 

	// this variable is used to store the worker ID of the current thread for local job execution
	JobSystem::JobSystem(uint32_t workerCount) noexcept {
		m_workerCount = workerCount ? workerCount : std::thread::hardware_concurrency();
		if (m_workerCount == 0) m_workerCount = 1; // for vms cases
		m_globalJobQueues = new(std::nothrow) Queue<Job>[m_workerCount];
		if (!m_globalJobQueues) {
			// TODO : fatal error
			assert(false);
		}
		m_localJobQueues = new(std::nothrow) Queue<Job>[m_workerCount];
		if (!m_localJobQueues) {
			// TODO : fatal error
			assert(false);
		}
		m_workerThreads = new(std::nothrow) std::thread[m_workerCount -1];
		if (!m_workerThreads) {
			//TODO : fatal error
			assert(false);
		}
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
		delete[] m_globalJobQueues;
		delete[] m_localJobQueues;
		delete[] m_workerThreads;
	}



	void JobSystem::AddJob(const Job& job) noexcept {

		// dont allow adding jobs after shutdown is signaled
		if (m_shutdown.load(std::memory_order_acquire)) {
			// TODO : Warning
			return;
		}

		// add one more job
		m_jobsInSystem.fetch_add(1, std::memory_order_relaxed);

		// 
		if (job.counter) {
			job.counter->fetch_add(1, std::memory_order_relaxed);
		}
		// if the job is affine then add it to the local queue of the worker with the specified ID
		if (job.affine) {
			uint32_t affineWID = job.affinityWorkerID;
			if (job.affinityWorkerID >= m_workerCount) {
				affineWID = m_workerCount - 1;
				// TODO : Warning 
			}
			m_localJobQueues[affineWID].Push(job);
		}
		else {
			
			uint32_t turn = m_QueueTurn.fetch_add(1, std::memory_order_relaxed);
			m_globalJobQueues[turn % m_workerCount].Push(job);
		}
	}

	void JobSystem::WaitFor(std::atomic<uint32_t>& counter) noexcept {
		bool localJob = false;
		
		while ((counter.load(std::memory_order_acquire) > 0)) {
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
				// TODO : fatal error , worker not allow to do this
				assert(false);
			}
		}
	}


	void JobSystem::ParallelFor(
		JobFunction func, void* data,
		uint32_t jobCount, std::shared_ptr<std::atomic<uint32_t>> counter) noexcept {
		if (jobCount == 0) {
			// TODO : fatal error
			return;
		}

		
		for (uint32_t i = 0; i < jobCount; ++i) {
			Job job;
			job.fun = func;
			job.data = data;
			job.affine = false;
			job.affinityWorkerID = 0; // not used since isAffine is false
			job.counter = counter;
			job.blockID = i;
			job.blockSize = jobCount;
			AddJob(job);
		}
	}

	void JobSystem::Shutdown(void) noexcept {
		m_shutdown.store(true, std::memory_order_release);

		if (std::this_thread::get_id() != m_mainThreadID) {
			// TODO : fatal error 
			assert(false);
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
		bool localJob = false;
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
			job.fun(job.data, workerID , job.GetID() , job.blockID , job.blockSize);
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
			if (m_localJobQueues[workerID].Pop(job)) {
				return true;
			}
			return false;
		}

		if (m_globalJobQueues[workerID].Pop(job)) {
			return true;
		}

		// Try steal job from other worker global queue
		
		for (uint32_t i = 0; i < m_workerCount; ++i) {
			if (i == workerID)
				continue;
			if (m_globalJobQueues[i].Pop(job)) {
				return true;
			}
		}
		
		return false;
	}

}
