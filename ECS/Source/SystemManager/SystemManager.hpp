#pragma once
#include "../JobSystem/JobSystem.hpp"
#include <Platform.hpp>
#include <unordered_map>
#include "../ISystem/ISystem.hpp"
/**
 * @author : Dimitris Smyrnakis
 * @file SystemManager.hpp
 * @brief The SystemManager is responsible for managing the execution of systems in different phases. 
 * It allows you to create phases, bind systems to those phases, and execute the systems in the 
 * correct order during each phase. The SystemManager also integrates with a JobSystem to enable 
 * parallel execution of systems when possible , as whell shares this job system with each system 
 * for use .
 */

namespace MultiStation{

	/**
	 * @class SystemManager
	 * @note The SystemManager should not be copied or moved, 
	 * as it manages resources and state that should not be duplicated.
	 */
	class SystemManager {
	public:
		
		/**
		 * @brief Constructs a new System Manager object. 
		 * Initializes the SystemManager and sets up necessary data structures for managing systems and phases. 
		 */
		SystemManager(void) noexcept;

		/**
		 * @brief Destroys the System Manager object and shutdown the job system .
		 */
		~SystemManager(void) noexcept;

		
		/**
		 * @warning Not allowed
		 */
		SystemManager(const SystemManager&) noexcept = delete;
		
		/**
		 * @warning Not allowed
		 */
		SystemManager& operator=(const SystemManager&) noexcept = delete;
		
		/**
		 * @warning Not allowed
		 */
		SystemManager(SystemManager&&) noexcept  = delete;
		
		/**
		 * @warning Not allowed
		 */
		SystemManager& operator=(SystemManager&&) noexcept = delete;
		

		/**
		 * .
		 * @brief Creates a new phase for system . For each phase a set of the added
		 * to this phase systems will execute all in parallel .
		 * @warning This method is used at initiallization of the systems for the game engine
		 * , if at least one call of ExecutePhase has been made before , then this method will assert 
		 * us for it , this is not allowed .
		 * @return A unique identifier for the newly created phase.
		 */
		uint32_t CreatePhase(void) noexcept;
		
		/**
		 * .
		 * @brief Binds systems to a specific phase. This allows you to specify which systems should be executed during a particular phase of the game loop.
		 * @param phaseID The unique identifier of the already created phase to bind systems to.
		 * @return nothing 
		 */
		void BindPhase(uint32_t phaseID) noexcept;

		/**
		 * .
		 * @brief Adds a system to the currently bound phase. The system will be executed during the execution of the currently bound phase.
		 * @note In Debug mode this method will assert if the same system is added more than once to the same phase .
		 * @param sys A shared ptr System to be added to the currently bounded phase . The system will be executed during the execution of the currently bounded phase.
		 * @warning This method is used at initiallization of the systems for the game engine
		 * , if at least one call of ExecutePhase has been made before , then this method will assert
		 * us for it , this is not allowed .
		 * @return nothing 
		 */
		void AddSystem(std::shared_ptr<ISystem> sys) noexcept;

		/**
		 * .
		 * @brief Removes a system from the currently bound phase. The system will no longer be executed during the execution of the currently bound phase.
		 * @param sys A shared ptr System to be removed from the currently bounded phase . The system will no longer be executed during the execution of the currently bounded phase.
		 * @warning This method is used at initiallization of the systems for the game engine
		 * , if at least one call of ExecutePhase has been made before , then this method will assert
		 * us for it , this is not allowed .
		 * @note If the system is not found in the currently bound phase or the currently bound phase does not exists
		 * , this method will assert .
		 * @return nothing
		 */
		void RemoveSystem(std::shared_ptr<ISystem> sys) noexcept;

		/**
		 * .
		 * @brief Executes all systems that are bound to the specified phase. 
		 * The systems will be executed in parallel if possible, using the JobSystem for scheduling and execution.
		 * @param phaseID The phase to execute . 
		 * @warning It should be a valid phase 
		 * that has been created before with CreatePhase method , otherwise this method will assert .
		 * @return 
		 */
		void ExecutePhase(uint32_t phaseID) noexcept;

		/**
		 * @brief Returns a list of all phase IDs that have been created in the SystemManager. 
		 * This allows you to query which phases are available for execution and management.
		 * 
		 * @return A list of all phase IDs that have been created in the SystemManager.
		 */
		std::vector<uint32_t> GetPhases() const noexcept;

		/**
		 * .
		 * @brief Returns a list of all systems that are bound to the specified phase. This allows you to query which systems are associated with a particular phase and manage them accordingly.
		 * @param phaseID The unique identifier of the phase to query.
		 * @return A list with all systems that are bound to the secified phase . If the phase does not exist or has no systems bound to it , an empty list is returned.
		 */
		std::vector<std::shared_ptr<ISystem>> GetSystemsInPhase(uint32_t phaseID) const noexcept;

	private:
		std::shared_ptr<JobSystem>								m_jobSystem;
		std::vector<uint32_t> 								    m_phases;
		std::unordered_map<uint32_t, 
			std::vector<std::shared_ptr<ISystem>> >				m_systems;
		uint32_t 												m_currentPhase;
		bool 													m_isExecuting;		
	};

}
