#pragma once
#include "ISystem.hpp"
#include <atomic>
namespace MultiStation {

	

	class ISystemModule {
	public:
		
		virtual ~ISystemModule(void) = default;


		/**
		 * @brief Virtual method for initiallize the system module , 
		 * things like views on editor registry , controllers for handling events services
		 * for the system to execute and communicate settings and properties 
		 * with the user via editor registry .
		 * 
		 * @param context It has all necessery engine context like scene (and in the future 
		 * editor and asset registry ) for the system loading / save / UI / scene operations.
		 * 
		 * @warning Do not use the scene to upload the module , for init to execute the module is
		 * already given to the scene .
		 */
		virtual void Init(EngineContext& context) = 0;

		/**
		 * @brief Virtual method for when the system module is unregister from the scene , this is
		 * where operations like editor.unregister , components unregister , assets shared sources 
		 * unregister must happen for the system completly remove it self from the scene 
		 * 
		 * @warning Do not use scene module unregister for this method to execute it is already called .
		 */
		virtual void Fini(EngineContext& context) = 0;

		/**
		 * @return It should return the ISystem implementation of this module  .
		 */
		virtual ISystem& GetSystem(void) = 0;

	};

	using ModuleID = uint32_t;
	extern std::atomic <ModuleID> moduleIdGenerator;
	template<typename T>
	ModuleID GetModuleID(void) {
		static ModuleID newID = moduleIdGenerator.fetch_add(1 , std::memory_order_relaxed);

		return newID;
	}

}
