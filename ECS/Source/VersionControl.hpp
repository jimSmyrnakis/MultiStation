#pragma once
#include <shared_mutex>
#include <stdint.h>
#include <stddef.h>
namespace MultiStation {

	class VersionControl {

	public:
		VersionControl() = default;
		~VersionControl() = default;

		VersionControl(const VersionControl&) = delete;
		VersionControl& operator=(const VersionControl&) = delete;

		VersionControl(VersionControl&& other) = delete;
		VersionControl& operator=(VersionControl&& other) = delete;
		
		// Thread safe
		uint32_t GetVersion(void) const;

		// Thread safe
		void IncrementVersion(void);

	private:
		mutable std::shared_mutex m_mutex;
		uint32_t m_version = 0;
	};
	
}
