#pragma once
#include <stdint.h>
#include <stddef.h>
#include <atomic>
#include <functional>
#include "../Serialization/IArchive.hpp"
#include <Platform.hpp>
namespace MultiStation{

	using CompID = uint64_t;
	using EntID = uint32_t;
	

	extern std::atomic<CompID> s_typeID ;

	extern EntID rootEntity;
	extern EntID nullEntity;
	
	
	/**
	 * @brief Gives for different types of components a unique id
	 * 
	 * \return  
	 */
	template<typename T>
	CompID GetComponentID(void) {
		static CompID id = ((uint64_t)1) <<
			s_typeID.fetch_add(
				1, 
				std::memory_order_relaxed
			);

		return id ;
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
		 * @brief Returns the serializer to register components and objects 
		 * for scene serialization.
		 */
		std::function<SerializationRegistry* ()> GetSerializer;


	};

}
