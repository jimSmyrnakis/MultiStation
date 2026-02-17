#pragma once
#include "Job.hpp"
#include <Utilities.hpp>
#include <thread>
/**
 * @author Dimitris Smyrnakis
 * @file JobSystem.hpp 
 * @brief A Simple for moment Job System implementation that schedules all job with fifo implementation hardcoded
 */
namespace MultiStation{

	static const uint32_t BAD_ID = 0XFFFFFFFF;

	/**
	 * @class JobSystem
	 * @brief Job System with fifo schedule algorithm . Supports local/affinity queues and global
	 * for scheduling the jobs base on some specific worker or global -> any worker available . The parent 
	 * Thread is part of the schedule algorithm , usefull method is the waitForAll that makes him enable
	 * for schedulling with the rest workers .
	 * @note Copy / Move Constructors / operators are not supported
	 */
	class JobSystem {


	public:
		/**
		 * 
		 * @brief Constructs the instance and created and start's all worker's ready to accept jobs
		 * in their queue's before the constructor finished 
		 * @param workerCount The ammount of threads will created to serve as workers . Zero if you want 
		 * to have as match workers as the available virtual cores of your cpu
		 * @note The Creator of this instance will consider the last worker by desing
		 *  
		 */
		JobSystem(uint32_t workerCount) noexcept;
		
		~JobSystem(void) noexcept;

		JobSystem(const JobSystem&) noexcept = delete;
		JobSystem(JobSystem&&)		noexcept = delete;

		JobSystem& operator=(const	JobSystem&) noexcept	= delete;
		JobSystem& operator=(JobSystem&&)		noexcept	= delete;
		

	public:

		/**
		 * 
		 * @brief Adds new job to any worker have turn (sequencial share)
		 * except if the job is affine then the affineWorkerID is used to define
		 * whos local/affine queue will take the job . If the job has affineWorkerID
		 * bigger that is not an actual worker ID then the parent thread worker takes
		 * that job and a warning is send . This method can be called even from threads 
		 * that are not workers .
		 * @param job the job we want to schedule
		 * 
		 */
		void AddJob(Job& job) noexcept;

		/**
		 * .
		 * @brief Waits Until the counter gets zero , then returns , until then this thread takes
		 * it's role as worker for schedulling jobs instead of doing nothing . This method can be
		 * called even from thread that is not worker , but then it will yield while waiting . 
		 * @param counter Atomic counter of the parallel group/block mostly .
		 * 
		 */
		void WaitFor(std::shared_ptr<std::atomic<uint32_t>>& counter) noexcept;
		
		/**
		 * @brief As WaitFor only that must called either from the parent thread 
		 * or from other not worker thread , otherwise fatal error is generated.
		 * 
		 */
		void WaitForAll(void) noexcept;

		/**
		 * .
		 * @brief Use this method for making block/group of jobs to do a specific task
		 * together . The counter can be even null , but the thread or job that does this 
		 * call can't know when this block of threads is finished
		 * @param[in] func The job function  
		 * @param[in] data The private data parsed to all the jobs of the group/block
		 * @param[in] jobCount The number of jobs this group/block has .
		 * @param[in] counter The common counter of this group/block used with WaitFor for synchronization
		 *  
		 */
		void ParallelFor(
			JobFunction func, void* data, uint32_t jobCount,
			std::shared_ptr<std::atomic<uint32_t>> counter) noexcept;

		/**
		 * @brief Used to terminate the hole job system so all worker threads stop executing
		 * and waits for them to join . 
		 * @note Only the owner/parent thread should call this , a job may succed or not if is 
		 * schedule by the parent worker/thread . But it should be avoided to used in a job .
		 * 
		 */
		void Shutdown(void) noexcept;

		
		

	private:
		void WorkerThread(uint32_t workerID) noexcept;
		void ExecuteJob(uint32_t workerID ,bool local) noexcept; // for main thread
		// executes all local jobs and 1 global job.
		bool TryPopOrStealJob(uint32_t workerID , Job& job , bool local) noexcept; // for worker threads
		void Schedule(uint32_t workerID) noexcept; // 
	private:
		uint32_t		m_workerCount;
		Queue<Job>*		m_gReadyQueues; // one queue per worker thread but can be shared
		Queue<Job>*		m_lReadyQueues; // one queue per worker thread but not shared
		std::thread*	m_workerThreads  ;
		std::thread::id	m_mainThreadID; // the main thread ID will also be used as checker
		std::atomic<bool>	m_shutdown = false; // flag to signal the worker threads to shutdown
		std::atomic<uint32_t> m_jobsInSystem = 0; // counter to keep track of the number of jobs
		std::atomic<uint32_t> m_QueueTurn = 0;
		// in the system for shotdown purposes, when this counter is 0 and shutdown flag is true then the worker threads can exit safely
		
	};

}
