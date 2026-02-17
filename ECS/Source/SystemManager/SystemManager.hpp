#pragma once
#include "../JobSystem/JobSystem.hpp"
#include <Platform.hpp>
#include <unordered_map>
#include "../ISystem/ISystem.hpp"
namespace MultiStation{

	class SystemManager {
	public:
		SystemManager(void) noexcept;
		~SystemManager(void) noexcept;
		SystemManager(const SystemManager&) noexcept = delete;
		SystemManager& operator=(const SystemManager&) noexcept = delete;
		SystemManager(SystemManager&&) noexcept = default;
		SystemManager& operator=(SystemManager&&) noexcept = default;
		

		uint32_t CreatePhase(void) noexcept;
		void BindPhase(uint32_t phaseID) noexcept;

		void AddSystem(std::shared_ptr<ISystem> sys) noexcept;
		void RemoveSystem(std::shared_ptr<ISystem> sys) noexcept;

		void ExecutePhase(uint32_t phaseID) noexcept;


	private:
		std::shared_ptr<JobSystem>								m_jobSystem;
		std::vector<uint32_t> 								    m_phases;
		std::unordered_map<uint32_t, 
			std::vector<std::shared_ptr<ISystem>> >				m_systems;
		uint32_t 												m_currentPhase;
		bool 													m_isExecuting;		
	};

}
