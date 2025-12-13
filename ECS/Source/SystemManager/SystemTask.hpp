#pragma once

#include "../Task/TaskManager.hpp"
#include "../ISystem/ISystem.hpp"
#include "../Registry/Registry.hpp"
#include <Platform.hpp>
#include <atomic>
namespace MultiStation {



	class SystemTask : public Task {

	public:
		enum State {
			Awake,
			Enable,
			Start,
			Update,
			Disable,
			Destroy
		};

		struct Info {
			std::atomic<State>* currentState;
			TaskManager* manager;
			ITaskMetadata* metadata;
			ISystem* system;
		};
		
	public:
		SystemTask(Info& info);
		SystemTask(const SystemTask& cpy) = delete;
		SystemTask& operator=(const SystemTask& cpy) = delete;
		SystemTask(SystemTask&& move) noexcept = delete;
		SystemTask& operator=(SystemTask&& move) noexcept = delete;
		~SystemTask(void) noexcept = default;

		void Run(uint16_t threadID) override;

	private:
		bool m_hasWake		= false;
		bool m_hasEnabled	= false;
		bool m_hasStarted	= false;
		bool m_hasDisabled	= false;
		bool m_hasDestroyed = false;
		bool m_hasUpdatedOnce = false;
		Info& m_info;
		float m_prevTime = 0.0f;
	};
}
