#include "HandleManager.hpp"
#include <atomic>
namespace MultiStation {

	namespace IO {

		int nextID = 0;
		std::mutex HandleManager::s_lock;
		int HandleManager::CreateHandle(void* native) {
			std::lock_guard<std::mutex> guard(s_lock);
			nextID++;
			int id = nextID;
			
			s_handles[id] = std::make_shared<Handle>();
			s_handles[id]->Lock();
			s_handles[id]->SetID( id);
			s_handles[id]->SetNative( native);
			s_handles[id]->Unlock();

			return id;
		}

		void HandleManager::RemoveHandle(int id) {
			std::lock_guard<std::mutex> guard(s_lock);
			auto it = s_handles.find(id);
			if (it != s_handles.end()) {
				s_handles.erase(it);
			}

		}

		std::shared_ptr<Handle> HandleManager::GetHandle(int id) {
			std::lock_guard<std::mutex> guard(s_lock);
			if (s_handles.count(id) != 0) {
				//s_handles[id]->Lock();
				return s_handles[id];
			}

			return nullptr;
		}

		void HandleManager::UnlockHandle(int id) {
			std::lock_guard<std::mutex> guard(s_lock);
			if (s_handles.count(id) != 0) {
				s_handles[id]->Unlock();
				return ;
			}

			return ;
		}

		bool HandleManager::HasHandle(int id) {
			std::lock_guard<std::mutex> guard(s_lock);
			return s_handles.count(id) != 0;
		}

		std::unordered_map<int, std::shared_ptr<Handle>> HandleManager::s_handles ;

	}

}
