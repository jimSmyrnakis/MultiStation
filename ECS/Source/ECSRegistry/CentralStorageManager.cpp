#include "CentralStorageManager.hpp"

namespace MultiStation {
	#define MAX_STORAGES 64
	ECSStorage storages[MAX_STORAGES];
	bool       available[MAX_STORAGES] = { true };
	uint16_t   availables = 64;
	ECSStorage* CentralStorageManager::s_storages = storages;
	uint16_t CentralStorageManager::s_total = MAX_STORAGES;
	bool is_init = false;

	void CentralStorageManager::Init(void) noexcept {
		if (!is_init) {
			for (int i = 0; i < MAX_STORAGES; i++)
			{
				available[i] = true;
			}
			availables = MAX_STORAGES;
			s_storages = storages;
			s_total = MAX_STORAGES;
			is_init = true;
		}
	}

	bool CentralStorageManager::IsInitialized(void) noexcept{
		return is_init;
	}

	bool CentralStorageManager::CreateStorage(
		size_t& id , 
		std::function<void(CompID)> at_unregister ) noexcept
	{
		if (!IsInitialized()) Init();

		// search for new storage
		for (uint16_t i = 0; i < MAX_STORAGES; i++) {
			if (available[i]) {
				id = i;
				available[i] = false;
				availables--;
				s_storages[i].at_unregister_call = at_unregister;
				return true;
			}
		}
		return false;
	}

	void CentralStorageManager::DestroyStorage(size_t id) noexcept {
		if (!IsInitialized()) {
			Init();
			return;
		}

		if (id >= MAX_STORAGES) return;
		if (!available[id]) {
			// clear all the space for each component
			for (auto& it : storages[id].components) {
				it.second->Clear();
			}
			available[id] = true;
			availables++;
			s_storages[id].at_unregister_call = nullptr;
		}
	}

	ECSStorage* CentralStorageManager::GetStorage(size_t id) noexcept {
		if (!IsInitialized()) {
			Init();
			return nullptr;
		}

		if (id >= MAX_STORAGES) return nullptr;

		if (!available[id]) return &storages[id];

		return nullptr;
	}

	uint16_t CentralStorageManager::GetTotalStorages(void) noexcept {
		return MAX_STORAGES;
	}

	uint16_t CentralStorageManager::GetTotalAvailableStorages(void) noexcept {
		return availables;
	}

	bool CentralStorageManager::HasRegister(CompID component) noexcept {
		if (!IsInitialized()) {
			Init();
			return false;
		}

		return s_storages[0].components.count(
			component
		);
	}

}
