#pragma once

#include <Platform.hpp>
#include <unordered_map>
#include "../Interfaces/IMSSystem.hpp"
#include "../Scene/Scene.hpp"
#include <mutex>
/**
 * @author Dimitris Smyrnakis
 * @file MSSystemManager.hpp
 * @date 2024-03-06 (yy-mm-dd)
 * @class MSSystemManager 
 * @brief This class used for managing systems that are updatable (have update methods that need's to be executed
 * like Script systems , Physics systems , animation systems etc. ) . It gives a way to define what systems will 
 * be executed in a single phase in parallel . Still the user must know what systems use the same components and
 * so he need to make systems executed in safe , without data races . 
 */

namespace MultiStation{

	
	class MSSystemManager {
	public:
		/**
		 * 
		 * 
		 * @param threads Number of threads that will run the system's
		 * 
		 */
		MSSystemManager(uint32_t threads) noexcept;
		~MSSystemManager(void) noexcept;

		MSSystemManager(const MSSystemManager&) noexcept = delete;
		MSSystemManager& operator=(const MSSystemManager&) noexcept = delete;

		MSSystemManager(MSSystemManager&&) noexcept = delete;
		MSSystemManager& operator=(MSSystemManager&&) noexcept = delete;

		/**
		 * 
		 * @brief Request a new phase to be created .
		 * @param phase The phase id
		 * 
		 */
		void CreatePhase(uint32_t phase) noexcept;
		/**
		 * .
		 * @brief Request a new phase to be destroyed (removing all the system's of this phase)
		 * @param phase The phase ID
		 *  
		 */
		void DestroyPhase(uint32_t phase) noexcept;

		/**
		 * 
		 * @brief Request the system to be added in that phase . A system can be added to multiple phase's
		 * too .
		 * @param sys The System implementation
		 * @param phase The phase ID
		 *  
		 */
		void AddSystem(IMSSystem* sys, uint32_t phase) noexcept;

		/**
		 *
		 * @brief Request the system to be removed from that phase .
		 * @param sys The System implementation
		 * @param phase The phase ID
		 *
		 */
		void RemoveSystem(IMSSystem* sys, uint32_t phase) noexcept;

		/**
		 * 
		 * 
		 * @brief The methods CreatePhase , DestroyPhase , AddSystem and RemoveSystem
		 * are just requesting something to happen . Here the request's are now actually happening. 
		 */
		void Update(void) noexcept;

		/**
		 * .
		 * @brief Start's the execution of the systems bound to the phase we desire .
		 * @param phase The phase ID
		 */
		void ExecutePhase(uint32_t phase) noexcept;

		/**
		 * @returns The job system reference that the MSSystemManager has created . 
		 */
		JobSystem& GetJobSystem(void) noexcept;
		/**
		 * @returns The job system constant reference that the MSSystemManager has created .
		 */
		const JobSystem& GetJobSystem(void) const noexcept;

		std::vector<uint32_t>::iterator begin(void);
		std::vector<uint32_t>::iterator end(void);

	private:

	private:
		enum RequestType {
			INSERT ,
			REMOVE 
		};
		struct Request {
			RequestType type;
			IMSSystem* system;
			uint32_t phaseID;
		};
		struct UpdateData {
			IMSSystem* system;
			float lastTime;
		};

	private:
		static void SystemCallBack(struct Job job);
		


	private:
		std::vector<IMSSystem*> m_systems; // all systems in the system manager
		std::vector<uint32_t> m_phases; // all phases in the system manager
		std::unordered_map<uint32_t, std::vector<IMSSystem*>> m_phaseSystemsMap; // map of phase id to systems in that phase
		std::unordered_map<uint32_t, std::vector<UpdateData>> m_phaseSystemUpdateDataMap; // map of phase id to update data for systems in that phase
		// This queue is thread safe
		Queue<Request> m_phaseRequestQueue; // queue of requests for phases 
		Queue<Request> m_systemRequestQueue; // queue of requests for systems
		// first is the phases and then the systems ;
	private:
		JobSystem* m_jobSystem; // pointer to the job system for scheduling system updates
		
	private:
		void Init(uint32_t threadsNum) noexcept; 
		void ProcessRequests(void) noexcept; // process the requests in the request queues
		void CreatePhase(Request request) noexcept; // create a new phase
		void RemovePhase(Request request) noexcept; // remove a phase
		void InsertSystem(Request request) noexcept; // insert a system into a phase
		void RemoveSystem(Request request) noexcept; // remove a system from a phase
		void Fini(void) noexcept; // clean up the system manager
	};

}
