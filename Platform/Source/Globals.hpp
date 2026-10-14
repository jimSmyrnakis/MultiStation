#pragma once
#include <stdint.h>
#include <stddef.h>
#include <atomic>
namespace MultiStation{

	using ObjID = uint32_t;
	extern std::atomic<ObjID> obj_id;
	template<typename T>
	ObjID GetObjectID(void) {
		static ObjID id = obj_id.fetch_add(
				1,
				std::memory_order_relaxed
			);

		return id;
	}

}
