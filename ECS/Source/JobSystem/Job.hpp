#pragma once
#include <stdint.h>
#include <stddef.h>
#include <atomic>
#include <memory>
#include <functional>
/**
 * @author Dimitris Smyrnakis
 */

 
namespace MultiStation{
	
	/**
	 * @brief A JobFunction is a function pointer that defines the signature of the function that can be executed as a job. 
	 * @param[in] data A pointer to the data that will be passed to the job function when it is executed. 
	 * @param[in] workerID The ID of the worker thread that is executing the job. 
	 * @param[in] id The ID of the job.
	 * @param[in] blockID The ID unique to the parallel for block that this job belongs to.
	 * @param[in] size The total number of jobs in the parallel for block that this job belongs to.
	 */
	typedef void (*JobFunction)(struct Job job);

	

	/**
	  * @brief A yield call back function type
	  */
	using JobYieldCallback = std::function<void()>;

	/**
	 *
	 * @brief A job is a unit of work that can be executed by a worker thread. The Job struct
	 * defines the function to execute and the attributes of the job .
	 * 
	 */
	struct Job {

		Job(void) noexcept;
		Job(const Job& cpy) noexcept = default;
		Job& operator=(const Job& cpy) noexcept = default;
		Job(Job&& move) noexcept = default;
		Job& operator=(Job&& move) noexcept = default;
		/**
		 * @var JobFunction fun
		 * @brief The function pointer that the worker thread will call when executing this job. 
		 */
		JobFunction								fun;

		/**
		 * @var void* data
		 * @brief The private data pointer that will be passed to the job function when it is executed. 
		 * These data is known only to the end user .
		 */
		void*									data;

		/**
		 * @var bool affine
		 * @brief If true the this job sould only be executed localy to a specific worker thread . Otherwise
		 * every worker thread randomly can pick it up for execution.
		 */
		bool									affine;

		/**
		 * @var uint32_t WorkerID
		 * @brief the ID of the worker thread that the job is executed.
		 */
		uint32_t								WorkerID; 

		/**
		 * @var std::shared_ptr<std::atomic<uint32_t>> counter 
		 * @brief For jobs that are part of group/block of jobs (e.g. parallel for) 
		 * this counter is used to sync the caller to the end of the group execution
		 */
		std::shared_ptr<std::atomic<uint32_t>>	counter; 

		char									name[64]; // for debugging purposes only
		
		/**
		 * @var uint32_t blockID
		 * @brief For jobs that are part of group/block of jobs (e.g. parallel for) this variable is used to identify the job in the block/group that belong .
		 */
		uint32_t								blockID; 

		/**
		 * @var uint32_t blockSize
		 * @brief For jobs that are part of group/block of jobs (e.g. parallel for) this variable is used to identify the total number of jobs in the block/group that belong .
		 */
		uint32_t 								blockSize;


		
		/**
		 * @brief Gets the unique identifier of the job.
		 * 
		 * \return the ID of the job.
		 */
		uint32_t GetID(void) const noexcept;




	private:
		static uint32_t s_nextID;
	private:
		uint32_t m_id;
	};



}
