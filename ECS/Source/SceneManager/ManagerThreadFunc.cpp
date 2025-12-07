#pragma once
#include "SceneManager.hpp"
#include <barrier>
namespace MultiStation{

	
	// This code will work on multiple threads , the only way to 
	void* systems_manager(void* info) {
		// input assignments
		std::vector<ISystem*>* inSystems = ((SceneManager::ManagerInfo*)info)->systems;
		Registry* registry = ((SceneManager::ManagerInfo*)info)->registry;
		std::shared_mutex* sharedLock = ((SceneManager::ManagerInfo*)info)->sharedLock;
		ManagerControlBlock* controlBlock = ((SceneManager::ManagerInfo*)info)->controlBlock;
		bool synch = ((SceneManager::ManagerInfo*)info)->synchronized;
		std::barrier<>* sharedBarrier = ((SceneManager::ManagerInfo*)info)->sharedBarrier;
		std::barrier<>* syncSharedBarrier = ((SceneManager::ManagerInfo*)info)->syncSharedBarrier;
		// TODO : Check all pointers 

		// Call the Create Method of each one of the assigned 
		// system's , warning this must be thread safe for each system
		sharedLock->lock();// one thread run's the creation methods of his
		// systems
		for (ISystem* system : *inSystems) {
			if (system == nullptr) continue;
			
			system->OnCreate(registry);
			
		}
		sharedLock->unlock(); // now other thread can start his system create method callback's

		// manager sync
		sharedBarrier->arrive_and_wait();
		float* UpTimers = new float[inSystems->size()];
		for (int i = 0;i < inSystems->size(); i++) {
			UpTimers[i] = Time::GetTimeInSeconds();
		}
		while (controlBlock->GetState() == ManagerState::UPDATE) {
			// while the manager need's to update the systems
			for (int i = 0; i < inSystems->size();i++) {
				ISystem* system = inSystems->at(i);
				if (system == nullptr) continue;

				//if the system writes to the registry
				if ((system->GetFlags() & ISystem::WRITABLE) == ISystem::WRITABLE) {
					sharedLock->lock(); // lock other readers and writers
					float oldTime = UpTimers[i];
					float now = Time::GetTimeInSeconds();
					float dt = now - oldTime;
					UpTimers[i] = now;
					system->OnUpdate(registry, dt, controlBlock);
					sharedLock->unlock(); // now new readers and writers are free to come

				}
				else { // if is only read

					sharedLock->lock_shared(); // every reader is free to come , a writer not 
					// , but if asked no new reader will come here 
					float oldTime = UpTimers[i];
					float now = Time::GetTimeInSeconds();
					float dt = now - oldTime;
					UpTimers[i] = now;
					system->OnUpdate(registry, dt, controlBlock);
					sharedLock->unlock_shared(); 
				}

			}
			if (synch) // game loop sync
				syncSharedBarrier->arrive_and_wait();
			else { // Give some time to the rest system's (Async Writer takes most time)
				//std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			
			
		}
		if (UpTimers)
			delete[] UpTimers;

		// manager sync
		sharedBarrier->arrive_and_wait();
		// call all destroy methods
		for (ISystem* system : *inSystems) {
			if (system == nullptr) continue;

			//if the system writes to the registry
			if ((system->GetFlags() & ISystem::WRITABLE) == ISystem::WRITABLE) {
				sharedLock->lock(); // lock other readers and writers
				system->OnDestroy(registry, controlBlock);
				sharedLock->unlock(); // now new readers and writers are free to come

			}
			else { // if is only read

				sharedLock->lock_shared(); // every reader is free to come , a writer not 
				// , but if asked no new reader will come here 
				system->OnDestroy(registry, controlBlock);
				sharedLock->unlock_shared();
			}
		}
		if (synch) // game loop sync
			syncSharedBarrier->arrive_and_wait();


		return nullptr;

	}

	


}
