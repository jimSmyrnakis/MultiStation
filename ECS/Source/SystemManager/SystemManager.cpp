#include "SystemManager.hpp"
#include <Utilities.hpp>
#include <Platform.hpp>
namespace MultiStation {
	
	SystemManager::SystemManager() noexcept {
		m_jobSystem = std::make_shared<JobSystem>(2.5f * std::thread::hardware_concurrency());
		MS_ASSERT(m_jobSystem, "Failed to create JobSystem.");
		m_ecsManager = std::make_shared<ECSManager>();
		MS_ASSERT(m_ecsManager, "Failed to create ECSManager.");
		m_currentPhase = 0;
		m_isExecuting = false;
	}

	SystemManager::~SystemManager(void) noexcept {
		m_jobSystem->Shutdown();
		m_systems.clear();
		m_phases.clear();
	}


	uint32_t SystemManager::CreatePhase(void) noexcept {
		MS_ASSERT(!m_isExecuting, "Cannot create phase while executing");
		uint32_t newPhaseID = m_phases.size() + 1; // start from 1
		m_phases.push_back(newPhaseID);
		return newPhaseID;
	}

	void SystemManager::BindPhase(uint32_t phaseID) noexcept {
		MS_ASSERT(!m_isExecuting, "Cannot bind phase while executing");
		auto it = std::find(m_phases.begin(), m_phases.end(), phaseID);
		
		MS_ASSERT((it != m_phases.end()), "Phase ID not found");
		m_currentPhase = phaseID;

	}

	void SystemManager::AddSystem(ISystem* sys) noexcept {
		MS_ASSERT(sys, "System cannot be null");
		MS_ASSERT(!m_isExecuting, "Cannot add system while executing");
#ifdef _DEBUG
		auto& vec = m_systems[m_currentPhase];
		MS_ASSERT(std::find(vec.begin(), vec.end(), sys) == vec.end(), "System already added");
#endif
		m_systems[m_currentPhase].push_back( sys);
	}

	void SystemManager::RemoveSystem(ISystem* sys) noexcept {
		MS_ASSERT(sys, "System cannot be null");
		MS_ASSERT(!m_isExecuting, "Cannot remove system while ticking");

		auto it = m_systems.find(m_currentPhase);
		MS_ASSERT((it != m_systems.end()), "Phase not found");

		auto& vec = it->second;
		auto vit = std::find(vec.begin(), vec.end(), sys);
		MS_ASSERT((vit != vec.end()), "System not found in phase");
		vec.erase(vit);
	}


	struct SystemTickData {
		ISystem* system;

		
	};

	void SystemCallBack(Job job) {
		auto* list = static_cast<SystemTickData*>(job.data);
		auto* data = list + job.blockID; // each job will execute one system
		data->system->OnUpdate(0.016);
	}

	void SystemManager::ExecutePhase(uint32_t phaseID) noexcept {
		m_isExecuting = true;

		// find all systems of the current phase and create SystemTickData for each system
		auto it = m_systems.find(phaseID);
		MS_ASSERT((it != m_systems.end()), "phase not Found");

		std::vector<SystemTickData> tickDataList;
		for (const auto& system : it->second) {
			SystemTickData tickData;
			tickData.system = system;
			tickDataList.push_back(tickData);
		}
		void* data = tickDataList.data();

		std::shared_ptr<std::atomic<uint32_t>>
			counter = std::make_shared<std::atomic<uint32_t>>(0);
		m_jobSystem->ParallelFor(SystemCallBack, data, tickDataList.size(), counter );
		m_jobSystem->WaitFor(counter);
		// update ecs manager
		m_ecsManager->PollOperations();
	}

	std::vector<uint32_t> SystemManager::GetPhases(void) const noexcept {
		return m_phases;
	}

	std::vector<ISystem*> SystemManager::GetSystemsInPhase(uint32_t phaseID) const noexcept {
		auto it = m_systems.find(phaseID);
		if (it != m_systems.end()) {
			return it->second;
		}
		return {};
	}

	JobSystem& SystemManager::GetJobSystem(void) noexcept {
		return *m_jobSystem;
	}

	const JobSystem& SystemManager::GetJobSystem(void) const noexcept {
		return *m_jobSystem;
	}

	ECSManager& SystemManager::GetECSManager(void) noexcept {
		return *m_ecsManager;
	}

	const ECSManager& SystemManager::GetECSManager(void) const noexcept {
		return *m_ecsManager;
	}


	std::vector<uint32_t>::iterator SystemManager::begin(void)noexcept {
		return m_phases.begin();
	}
	std::vector<uint32_t>::iterator SystemManager::end(void)noexcept {
		return m_phases.end();
	}
}
