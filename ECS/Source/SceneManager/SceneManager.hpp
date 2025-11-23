#pragma once
#include "../ComponentRegistry/ComponentRegistry.hpp"
#include <shared_mutex>
#include <thread>
#include "../ISystem/ISystem.hpp"
namespace MultiStation{

	class SceneManager {




	private:
		ComponentRegistry* m_registry;
		// Registry of the Scene
		std::shared_mutex m_sharedMutex;
		// If One System Writes then the all waiting
		// If One or more System's Reads then all Write Flaged System's waiting to take turn
		std::vector<ISystem*> m_Systems;
		// A list of All System's 
		std::vector<std::thread*> m_Threads;
		// A list of All Available Threads
		// We Run At the Entry Point Thread , so no Thread safety for us needed
		std::unordered_map<std::thread*, std::vector<ISystem*>> m_Assingments;
		// A map with the assinged Systems per thread , defined only by the 
		bool isDirty; 
		// True if previus system change the registry , this helps the rest system's to update them selfs
		// properly , for example a Editor System may need that to update the requested Gui for the user 
		// as well the entities shown on the scene that the user interact's with .
	};

}
