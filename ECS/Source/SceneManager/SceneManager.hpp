#pragma once
#include "../Registry/Registry.hpp"
#include <shared_mutex>
#include <thread>
#include <barrier>
#include "../ISystem/ISystem.hpp"
#include "../ManagerControlBlock/ManagerControlBlock.hpp"
#include <Platform.hpp>
namespace MultiStation{

	void* systems_manager(void* info);
	class SceneManager {
	public:
		struct ManagerInfo {
			Registry* registry;
			// registry that each thread will use 
			std::vector<ISystem*>* systems;
			// All System's a thread will use 
			std::shared_mutex* sharedLock; 
			// The state of the manager
			ManagerControlBlock* controlBlock;
			// Manager Control Block
			bool synchronized; // that says that the system's you have will not 
			// need to ynchronized with the game loop
			std::barrier<>* sharedBarrier; 
			// Shared barrier for manager synchronization
			std::barrier<>* syncSharedBarrier; 
			// Shared Barrier for game loop sunc
		};

		

	private:
		SceneManager(void) noexcept;
		~SceneManager(void) noexcept;

		

		void Release(void) noexcept;

	private:

		void ThreadPoolInit(void);
		void ThreadPoolFini(void);
		void RunThreads(void);
		void CloseThreads(void);

	public:

		static SceneManager* Create(void) noexcept;
		static void Destroy(SceneManager* ptr) noexcept;
		
		
		SceneManager(const SceneManager& other) noexcept = delete;
		SceneManager& operator=(const SceneManager& other) noexcept = delete;
		SceneManager(SceneManager&& other) noexcept = delete;
		SceneManager& operator=(SceneManager&& other) noexcept = delete;

	public:

		uint32_t GetSyncThreadsNum(void) const;
		uint32_t GetAsyncThreadsNum(void) const;

		void AssignSyncSystem(uint32_t syncThreadIndex , ISystem* system);
		void AssignAsyncSystem(uint32_t asyncThreadIndex, ISystem* system);

		void StartSystems(void);
		void StopSystems(void);






	private:
		Registry* m_registry;
		// Registry of the Scene
		std::shared_mutex* m_sharedMutex;
		// If One System Writes then the all waiting
		// If One or more System's Reads then all Write Flaged System's waiting to take turn
		std::barrier<>* m_sharedBarrier; 
		// shared barrier for the manager 
		std::barrier<>* m_syncSharedBarrier;
		// shared barrier for the scene loop 
		std::vector<ISystem*>* m_Systems;
		// A list of All System's 
		std::vector<std::thread>* m_Threads;
		// A list of All Available Threads
		// We Run At the Entry Point Thread , so no Thread safety for us needed
		std::unordered_map<size_t, std::vector<ISystem*>> m_Assingments;
		// A map with the assinged Systems per thread , defined only by the end user
		ManagerControlBlock  m_ControlBlock ;// defines the version the state of the registry is
		// for example let's say system x is WRITABLE and change's the version count to
		// versionCount++; then every other system that is WRITABLE or not see's that and says
		// my last versionCount is less than the new one , i will reload everything as removing an
		// component changes the all other component's addresses (not a good one but that was fast to made)
		// that means every system must reload everything with the new one addresses
		uint32_t m_threadsCount = 0;
	};

}
