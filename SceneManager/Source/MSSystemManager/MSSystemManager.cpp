#include "MSSystemManager.hpp"
#include <Utilities.hpp>
#include <Platform.hpp>
namespace MultiStation {

	MSSystemManager::MSSystemManager(uint32_t threads) noexcept {
		Init(threads);
	}
	MSSystemManager::~MSSystemManager(void) noexcept {
		Fini();
	}


	void MSSystemManager::CreatePhase(uint32_t phase) noexcept {
		Request request;
		request.phaseID = phase;
		request.type = INSERT;
		m_phaseRequestQueue.Push(request);
	}
	void MSSystemManager::DestroyPhase(uint32_t phase) noexcept {
		Request request;
		request.phaseID = phase;
		request.type = REMOVE;
		m_phaseRequestQueue.Push(request);
	}

	void MSSystemManager::AddSystem(IMSSystem* sys, uint32_t phase) noexcept {
		Request request;
		request.phaseID = phase;
		request.system = sys;
		request.type = INSERT;
		m_systemRequestQueue.Push(request);
	}
	void MSSystemManager::RemoveSystem(IMSSystem* sys, uint32_t phase) noexcept {
		Request request;
		request.phaseID = phase;
		request.system = sys;
		request.type = REMOVE;
		m_systemRequestQueue.Push(request);
	}

	void MSSystemManager::Update(void) noexcept {
		this->ProcessRequests();
	}



	void MSSystemManager::ExecutePhase(uint32_t phase) noexcept {
		// check if phase exist's
		if (std::find(m_phases.begin(), m_phases.end(), phase) == m_phases.end()) {
			MS_ENGINE_WARN("Phase %d does not exist's in the system manager phase list", phase);
			return;
		}

		// check if exist's in the phase system map and update data system phase map
		if (m_phaseSystemsMap.find(phase) == m_phaseSystemsMap.end()) {
			MS_ENGINE_WARN("Phase %d does not exists in the phase,system map", phase);
			return;
		}
		if (m_phaseSystemUpdateDataMap.find(phase) == m_phaseSystemUpdateDataMap.end()) {
			MS_ENGINE_WARN("Phase %d does not exists in the phase,SystemUpdateData map", phase);
			return;
		}

		// for each system in the phase and each update data
		std::vector<UpdateData>& data = m_phaseSystemUpdateDataMap[phase];
		int cnt = data.size();
		std::shared_ptr<std::atomic<uint32_t>> counter = std::make_shared<std::atomic<uint32_t>>(0);
		m_jobSystem->ParallelFor(MSSystemManager::SystemCallBack
			, data.data(), cnt, counter);
		m_jobSystem->WaitFor(counter);

	}


	void MSSystemManager::SystemCallBack(Job job) {
		// get the phase id and system from the job data
		UpdateData* list = (UpdateData*)job.data;
		UpdateData& updateData = list[job.blockID];
		IMSSystem* system = updateData.system;

		float now = Time::GetTimeInSeconds();

		float deltaTime = 0.0f;
		if (updateData.lastTime > 0.0f)
			deltaTime = now - updateData.lastTime;

		updateData.lastTime = now;
		system->OnUpdate(deltaTime);
	}

	JobSystem& MSSystemManager::GetJobSystem(void) noexcept { return *m_jobSystem; }
	
	const JobSystem& MSSystemManager::GetJobSystem(void) const noexcept { return *m_jobSystem; }

	std::vector<uint32_t>::iterator MSSystemManager::begin(void) { return m_phases.begin(); }

	std::vector<uint32_t>::iterator MSSystemManager::end(void) { return m_phases.end(); }






























	void MSSystemManager::Init(uint32_t count) noexcept {
		m_jobSystem = new JobSystem(count);
		
		
	}
	void MSSystemManager::ProcessRequests(void) noexcept {
		Request request;
		// for each request in the phase request queue
		while (m_phaseRequestQueue.Pop(request)) {
			switch (request.type) {
			case RequestType::INSERT:
				CreatePhase(request);
				break;
			case RequestType::REMOVE:
				RemovePhase(request);
				break;
			default:
				MS_ENGINE_WARN("Unknown request type in phase request queue");
				break;
			}
		}

		// For each request in the system request queue
		while (m_systemRequestQueue.Pop(request)) {
			switch (request.type) {
			case RequestType::INSERT:
				InsertSystem(request);
				break;
			case RequestType::REMOVE:
				RemoveSystem(request);
				break;
			default:
				MS_ENGINE_WARN("Unknown request type in phase request queue");
				break;
			}
		}
	}
	void MSSystemManager::CreatePhase(Request request) noexcept {
		// check if the phase already exists
		if (std::find(m_phases.begin(), m_phases.end(), request.phaseID) != m_phases.end()) {
			MS_ENGINE_WARN("Phase with id %d already exists", request.phaseID);
			return;
		}
		m_phases.push_back(request.phaseID);
		m_phaseSystemsMap[request.phaseID] = std::vector<IMSSystem*>();
		m_phaseSystemUpdateDataMap[request.phaseID] = {};
	}
	void MSSystemManager::RemovePhase(Request request) noexcept {
		// check if the phase exists
		auto it = std::find(m_phases.begin(), m_phases.end(), request.phaseID);
		if (it == m_phases.end()) {
			MS_ENGINE_WARN("Phase with id %d does not exist", request.phaseID);
			return;
		}
		m_phases.erase(it);
		m_phaseSystemsMap.erase(request.phaseID);
		m_phaseSystemUpdateDataMap.erase(request.phaseID);
	}
	void MSSystemManager::InsertSystem(Request request) noexcept {
		// check if the phase exists
		auto it = std::find(m_phases.begin(), m_phases.end(), request.phaseID);
		if (it == m_phases.end()) {
			MS_ENGINE_WARN("Phase with id %d does not exist", request.phaseID);
			return;
		}
		m_phaseSystemsMap[request.phaseID].push_back(request.system);
		m_systems.push_back(request.system);
		m_phaseSystemUpdateDataMap[request.phaseID].emplace_back(request.system, 0.0f);
	}
	void MSSystemManager::RemoveSystem(Request request) noexcept {
		// check if the phase exists
		auto it = std::find(m_phases.begin(), m_phases.end(), request.phaseID);
		if (it == m_phases.end()) {
			MS_ENGINE_WARN("Phase with id %d does not exist", request.phaseID);
			return;
		}
		auto& systems = m_phaseSystemsMap[request.phaseID];
		auto systemIt = std::find(systems.begin(), systems.end(), request.system);
		if (systemIt == systems.end()) {
			MS_ENGINE_WARN("System %s does not exist in phase %d", request.system->GetName().c_str(), request.phaseID);
			return;
		}

		//check if the system already exists in the system manager
		auto systemslistit = std::find(m_systems.begin(), m_systems.end(), request.system);
		if (systemslistit == m_systems.end()) {
			MS_ENGINE_WARN("System %s already does not exists in the system manager", request.system->GetName().c_str());
			return;
		}
		systems.erase(systemIt);
		m_systems.erase(systemslistit);
		auto& updateDataVec = m_phaseSystemUpdateDataMap[request.phaseID];
		auto updateDataIt = std::find_if(updateDataVec.begin(), updateDataVec.end(),
			[request](UpdateData& data) {
				return data.system == request.system;
			});
		if (updateDataIt != updateDataVec.end()) {
			updateDataVec.erase(updateDataIt);
		}
	}
	void MSSystemManager::Fini(void) noexcept {
		if (m_jobSystem) {
			m_jobSystem->Shutdown();
			delete m_jobSystem;
			m_jobSystem = nullptr;
		}
		m_phases.clear();
		m_systems.clear();
		m_phaseSystemsMap.clear();
		m_phaseSystemUpdateDataMap.clear();
		m_systemRequestQueue.Clear();
	}


	
}
