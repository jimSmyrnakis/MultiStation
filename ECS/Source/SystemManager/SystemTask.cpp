#include "SystemTask.hpp"

namespace MultiStation {

	SystemTask::SystemTask(Info& info) : m_info(info) {
		m_hasDestroyed =
			m_hasDisabled =
			m_hasEnabled =
			m_hasWake =
			m_hasStarted =
			m_hasUpdatedOnce = 
			false;

		// load to manager
		info.manager->AddTask(this, info.metadata);
	}

	void SystemTask::Run(uint16_t threadid) {
		// take state 
		State state = m_info.currentState->load(std::memory_order_acquire);
		ISystem* sys = m_info.system;
		switch (state)
		{
		case MultiStation::SystemTask::Awake:
			if (!m_hasWake)
			{
				sys->OnAwake();
				m_hasUpdatedOnce = false;
				m_hasWake = true;
				m_hasDestroyed = false;
				m_hasStarted = false;
				m_hasEnabled = false;
				m_hasDisabled = false;
			}
			break;
		case MultiStation::SystemTask::Enable:
			if (!m_hasEnabled) {
				sys->OnEnable();
				m_hasUpdatedOnce = false;
				m_hasWake = false;
				m_hasDestroyed = false;
				m_hasStarted = false;
				m_hasEnabled = true;
				m_hasDisabled = false;
			}
			break;
		case MultiStation::SystemTask::Start:
			if (!m_hasStarted) {
				sys->OnStart();
				m_hasUpdatedOnce = false;
				m_hasWake = false;
				m_hasDestroyed = false;
				m_hasStarted = true;
				m_hasEnabled = false;
				m_hasDisabled = false;
			}
			break;
		case MultiStation::SystemTask::Update:
			float curTime = Time::GetTimeInSeconds();
			if (!m_hasUpdatedOnce) {
				m_hasUpdatedOnce = true;
				m_hasWake = false;
				m_hasDestroyed = false;
				m_hasStarted = true;
				m_hasEnabled = false;
				m_hasDisabled = false;
				m_prevTime = curTime;
			}
			sys->OnUpdate(curTime - m_prevTime);
			m_prevTime = curTime;
			break;
		case MultiStation::SystemTask::Disable:
			if (!m_hasDisabled) {
				sys->OnDisable();
				m_hasUpdatedOnce = false;
				m_hasWake = false;
				m_hasDestroyed = false;
				m_hasStarted = false;
				m_hasEnabled = false;
				m_hasDisabled = true;
			}
			break;
		case MultiStation::SystemTask::Destroy:
			if (!m_hasDestroyed) {
				sys->OnDestroy();
				m_hasUpdatedOnce = false;
				m_hasWake = false;
				m_hasDestroyed = true;
				m_hasStarted = false;
				m_hasEnabled = false;
				m_hasDisabled = false;
			}
			break;
		default:
			break;
		}
	}
}
