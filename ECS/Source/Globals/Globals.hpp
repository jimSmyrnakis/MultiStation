#pragma once
#include <stdint.h>
#include <stddef.h>
#include <atomic>
namespace MultiStation{

	using CompID = uint64_t;
	using EntID = uint32_t;
	using ObjID = uint32_t;

	extern std::atomic<CompID> s_typeID ;

	extern EntID rootEntity;
	extern EntID nullEntity;
	extern std::atomic<ObjID> obj_id;   
	
	/**
	 * @brief Gives for different types of components a unique id
	 * 
	 * \return 
	 */
	template<typename T>
	CompID GetComponentID(void) {
		static CompID id = 1 <<
			s_typeID.fetch_add(
				1, 
				std::memory_order_relaxed
			);

		return id ;
	}

	template<typename T>
	ObjID GetObjectID(void) {
		static CompID id = 1 <<
			obj_id.fetch_add(
				1,
				std::memory_order_relaxed
			);

		return id;
	}

	class Scene;
	struct EngineContext {

		Scene* scene;
		// in the future will have editor registry , asset registry
		// windows and other engine needed systems and things
	};

}
