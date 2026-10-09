#pragma once
#include <stdint.h>
#include <stddef.h>
#include <atomic>
#include <functional>
#include "../Serialization/Archive/IArchive.hpp"
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

	using ModuleID = uint32_t;
	extern std::atomic <ModuleID> moduleIdGenerator;
	template<typename T>
	ModuleID GetModuleID(void) {
		static ModuleID newID = moduleIdGenerator.fetch_add(1, std::memory_order_relaxed);

		return newID;
	}

	class Scene;
	class SerializationRegistry;
	struct EngineContext {

		/**
		 * @brief Returns the current scene that is loaded in the engine
		 */
		std::function<Scene* ()> GetCurrentScene;

		/**
		 * @brief Returns the serialization registry of the engine
		 */
		std::function<SerializationRegistry& ()> GetSerializer;

		/**
		 * @brief Loads a scene from the given archive
		 */
		std::function<bool(IArchiveReader* )> LoadScene;

		/**
		 * @brief Saves the current scene to the given archive
		 */
		std::function<bool(IArchiveWriter*)> SaveScene;



	};

}
